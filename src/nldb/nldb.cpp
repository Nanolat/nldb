/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the NOSL(Nanolat Open Source License) v1.0.
 * 
 * Simple Description :
 * - If you are in USA, Canada, or Germany, it is *likely* that you get free LGPL v2.1.
 * - In other countries, you are *likely* under GPL v3, but you can by LGPL v2.1 from Nanolat Co., Ltd.
 * - For free software, you *definitely* get free LGPL v2.1 whereever you are.
 * 
 * Detailed Description :
 * - In the hope of seeing increasing open source contributions in countries 
 *   that do not have enough open source contributors, Nanolat Open Source License 
 *   provides free LGPL v2.1 only to companies and people of countries 
 *   that have at least three KudoRank 10 contributors in http://www.ohloh.net/people. 
 *   The list of countries are updated by the beginning of a year. 
 *   In 2013, these countries are USA, Canada, and Germany.
 *
 * - For companies and people of other countries, you are under GPL v3.0, 
 *   but you can buy LGPL v2.1 license for a product from Nanolat Co., Ltd.
 *
 * - However, for non-commercial usage, you get free LGPL v2.1 whereever you are.
 *
 * - A company is of a country if the head of office is in the country.
 *
 * - A person is of a country if the nationality of the person is the country.
 *
 * - A country has a KudoRank 10 contributor 
 *   if the contributor's location is the country in http://www.ohloh.net/.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *
 * Contributors:
 *     Kangmo Kim
 */


#include <txbase/tx_assert.h>

#include <nldb/nldb.h>
#include <nldb/nldb_plugin.h>
#include "nldb_internal.h"

#if defined(_MSC_VER)
#  include <boost/atomic.hpp>
#else
#  include <atomic>
#endif

#include <boost/thread/thread.hpp>
typedef boost::thread thread_t;

#include <disruptor/ring_buffer.h>
#include <disruptor/event_publisher.h>
#include <disruptor/event_processor.h>
#include <disruptor/exception_handler.h>

#include "nldb_txevent.h"
#include "nldb_plugin_leveldb.h"
#include "nldb_plugin_tc.h"
#include "nldb_plugin_array_tree.h"
#include "nldb_repl_slave_thread.h"

using namespace disruptor;

// BUGBUG : 
// if we set NLDB_TX_EVENT_COUNT to 4, and set #define TEST_TRANSACTIONS (100000000L) in llcep_nldb_server.cpp, the slave does not receive all replication messages.
// Need to check why.
//#define NLDB_TX_EVENT_COUNT (4)

#define NLDB_TX_EVENT_COUNT (1024 * 32)

/*********************/
/* plugin management */
/*********************/
#define NLDB_PLUGIN_MAX_COUNT (16)
static nldb_plugin_t * plugins[NLDB_PLUGIN_MAX_COUNT];
static nldb_plugin_id_t next_plugin_id = 0;
// return the maximum number of plugins
nldb_uint32_t nldb_plugin_max_count()
{
	return NLDB_PLUGIN_MAX_COUNT;
}

// errors : NLDB_ERROR_PLUGIN_NO_MORE_SLOT
nldb_rc_t nldb_plugin_add( nldb_plugin_t & plugin, nldb_plugin_id_t * plugin_id )
{
	if ( next_plugin_id >= NLDB_PLUGIN_MAX_COUNT )
		return NLDB_ERROR_PLUGIN_NO_MORE_SLOT;

	nldb_plugin_id_t the_plugin_id = next_plugin_id++;
	
	plugins[the_plugin_id] = & plugin;

	*plugin_id = the_plugin_id;

	return NLDB_OK;
}

static inline nldb_plugin_t * nldb_plugin( const nldb_plugin_id_t & id) {
	tx_assert(id < NLDB_PLUGIN_MAX_COUNT );
	return plugins[id];
}


// Default table plugins
nldb_plugin_id_t NLDB_TABLE_PERSISTENT;
nldb_plugin_id_t NLDB_TABLE_VOLATILE;
nldb_plugin_id_t NLDB_TABLE_TC;

// The default table plugin for volatile tables ; uses TokyoCabinet
nldb_plugin_tc_t      plugin_tc;

nldb_plugin_array_tree_t      plugin_array_tree;

// The default table plugin for persistent tables ; uses LevelDB
nldb_plugin_leveldb_t plugin_leveldb;

// Initialize LLDB system.
nldb_rc_t nldb_init()
{
	nldb_rc_t  rc;
	rc = nldb_plugin_add( plugin_leveldb, & NLDB_TABLE_PERSISTENT );
	if ( rc ) return rc;

	rc = nldb_plugin_add( plugin_array_tree, & NLDB_TABLE_VOLATILE );
	if ( rc ) return rc;

	rc = nldb_plugin_add( plugin_tc, & NLDB_TABLE_TC );
	if ( rc ) return rc;

	return NLDB_OK;
}

// Destroy LLDB system.
nldb_rc_t nldb_destroy()
{
	// Remove all plugins
	next_plugin_id = 0;
	// do nothing for now.
	return NLDB_OK;
}


/******************/
/* opaque structs */
/******************/

class TxTransactionsLogs
{
public : 
	TxTransactionsLogs() 
		:  sequence_to_track(0)
	{
		txRingBuffer = new RingBuffer<TxTransactionEvent>(&txEventFactory,
                               NLDB_TX_EVENT_COUNT,
                               kSingleThreadedStrategy,
                               kBusySpinStrategy);

		currentSequence = -1;
	};

	~TxTransactionsLogs()
	{
		delete txRingBuffer;
	}

	class Replicator {
	public :
		Replicator(const nldb_db_id_t & dbid, TxTransactionsLogs * outer, const std::string & master_ip, const unsigned short master_port) :
		   	logPublisher(dbid, master_ip, master_port),
			first_barrier( outer->txRingBuffer->NewBarrier(outer->sequence_to_track) ),
			first_processor( outer->txRingBuffer,
                             (SequenceBarrierInterface*) first_barrier.get(),
                             &logPublisher,
                             &exception_handler),
			first_consumer(std::ref<BatchEventProcessor<TxTransactionEvent>>(first_processor))
			{
			}

		inline BatchEventProcessor<TxTransactionEvent> & processor() 
		{
			return first_processor;
		}

		inline thread_t & consumer() 
		{
			return first_consumer;
		}
	private :
		std::unique_ptr<ProcessingSequenceBarrier> first_barrier;

		BatchEventProcessor<TxTransactionEvent> first_processor;

		// one exception handler
		IgnoreExceptionHandler<TxTransactionEvent> exception_handler;
		// one event handler
		TxTransactionLogPublisher logPublisher;

		thread_t first_consumer;
	};

	inline void addReplicator(const nldb_db_id_t & dbid, const std::string & master_ip, const unsigned short master_port)
	{
		std::shared_ptr<Replicator> replicator ( new Replicator( dbid, this, master_ip, master_port ) );
		replicators_.push_back(replicator);

		sequence_to_track.clear();
			    
		sequence_to_track.push_back(replicator->processor().GetSequence());

		// Set gating sequences to prevent the buffer wrapping.
		// BUGBUG : Think if this is ok even though we have multiple Replicators.
		txRingBuffer->set_gating_sequences(sequence_to_track);
	};
	
	inline bool hasReplicators() {
		return ! replicators_.empty();
	}

	inline void destroy() {
		for (std::vector<std::shared_ptr<Replicator>>::iterator it = replicators_.begin();
			it != replicators_.end();
			it ++ )
		{
			std::shared_ptr<Replicator> replicator = *it;
			Replicator * r = replicator.get();
			r->processor().Halt();
			r->consumer().join();
		}
	};

	inline TxTransactionEvent * getNextTransactionEvent() {

        int64_t sequence = txRingBuffer->Next();
		tx_assert( sequence >= 0 );

		TxTransactionEvent * txEvent = txRingBuffer->Get(sequence);
        translator.TranslateTo(sequence, txEvent);

		// currentSequence will be used to publish the sequence.
		currentSequence = sequence;

		return txEvent;
	};

	// Return current uncommited transaction event if any.
	// Return NULL otherwise.
	inline TxTransactionEvent * getCurrentTransactionEvent() {
		if (currentSequence >= 0) {
			TxTransactionEvent * txEvent = txRingBuffer->Get(currentSequence);
			return txEvent;
		}
		return NULL;
	};

	inline void commitCurrentTransactionEvent() {
		tx_assert(currentSequence >= 0);
        txRingBuffer->Publish(currentSequence);
		currentSequence = -1;
	};

	inline void waitForReplicationPublishers()
	{
	    long expected_sequence = txRingBuffer->GetCursor();

		for(int i=0; i<replicators_.size(); i++) 
		{
			BatchEventProcessor<TxTransactionEvent> & processor = replicators_[i]->processor();
			while (processor.GetSequence()->sequence() < expected_sequence) {}
		}
	}

private :
	// A replicator replicates data to a slave. 
	// A master can have multiple slaves in this vector.
	std::vector<std::shared_ptr<Replicator>> replicators_;


	RingBuffer<TxTransactionEvent> * txRingBuffer;
    TxTransactionEventFactory txEventFactory;

    std::vector<Sequence*> sequence_to_track;

	// Publisher
	TxTransactionEventTranslator translator;

	// the sequence that is not published yet.
	int64_t currentSequence;
};

// Each DB has a meta table, whose table id is 0. 
// The meta table contains list of tables.
// The key of the meta table is table id of a table in the DB.
// The value of the meta table is following structure.
typedef struct nldb_meta_table_value_t {
	nldb_plugin_id_t         table_plugin_id;
	nldb_plugin_table_desc_t plugin_table_desc;
} nldb_meta_table_value_t;

const nldb_table_id_t META_TABLE_ID = 0;

class nldb_db_impl_t
{
public : 
	nldb_db_impl_t()
	{
		db_id_ = 0;
		meta_table_context_ = NULL;
	};

	inline nldb_rc_t init(const nldb_db_id_t & db_id) {
		meta_table_desc_ = nldb_meta_table_desc(db_id);
		db_id_ = db_id;

		nldb_rc_t rc = meta_table_plugin_.table_open(meta_table_desc_, & meta_table_context_);
		return rc;
	};

	inline nldb_rc_t destroy() {
		tx_logs_.destroy();

		nldb_rc_t rc = meta_table_plugin_.table_close( meta_table_context_);
		return rc;
	};

	// create the meta table.
	inline static nldb_rc_t meta_create(const nldb_db_id_t & db_id) {
		nldb_plugin_table_desc_t dummy_table_desc;
		nldb_plugin_leveldb_t meta_table_plugin;
		return meta_table_plugin.table_create(db_id, META_TABLE_ID, &dummy_table_desc);
	}

	// drop the meta table. removes meta table files from disk.
	inline static nldb_rc_t meta_drop(const nldb_db_id_t & db_id) {
		nldb_plugin_table_desc_t meta_table_desc = nldb_meta_table_desc(db_id);
		nldb_plugin_leveldb_t meta_table_plugin;
		return meta_table_plugin.table_drop(meta_table_desc);
	}


	// put a record(table description) into the meta table
	inline nldb_rc_t meta_put(const nldb_table_id_t & table_id, const nldb_plugin_id_t & table_plugin_id, const nldb_plugin_table_desc_t & table_desc) {
		nldb_meta_table_value_t meta_table_value;
		meta_table_value.table_plugin_id = table_plugin_id;
		meta_table_value.plugin_table_desc = table_desc;
		nldb_key_t key     = {(void*)&table_id, sizeof(table_id)};
		nldb_value_t value = {(void*)&meta_table_value, sizeof(meta_table_value)};
		// assumption : the key and value is copied when they are stored.
		return meta_table_plugin_.table_put( meta_table_context_, key, value);
	}

	// get a record(table description) from the meta table
	inline nldb_rc_t meta_get(const nldb_table_id_t & table_id, nldb_plugin_id_t * table_plugin_id, nldb_plugin_table_desc_t * table_desc) {
		nldb_key_t   key     = {(void*)&table_id, sizeof(table_id)};
		nldb_value_t value;

		nldb_rc_t rc = meta_table_plugin_.table_get( meta_table_context_, key, & value);
		if ( rc ) return rc;

		nldb_meta_table_value_t * meta_table_value = (nldb_meta_table_value_t *) value.data ;

		*table_plugin_id = meta_table_value->table_plugin_id;
		memcpy(table_desc,  & meta_table_value->plugin_table_desc, sizeof(meta_table_value->plugin_table_desc));

		return meta_table_plugin_.value_free( value );
	}
	// delete a record(table description) from the meta table
	inline nldb_rc_t meta_del(const nldb_table_id_t & table_id) {
		nldb_key_t   key     = {(void*)&table_id, sizeof(table_id)};

		return meta_table_plugin_.table_del( meta_table_context_, key);
	}

	// Add a slave for this DB. All comitted data will be replicated to the slave.
	inline void add_replicator(const nldb_db_id_t & dbid, const std::string & master_ip, const unsigned short master_port)
	{
		tx_logs_.addReplicator(dbid, master_ip, master_port);
	}

	// keeps logs of all transactions to send to slave. Implemented w/ a circular queue.
    TxTransactionsLogs tx_logs_;

	inline nldb_db_id_t db_id() const {
		return db_id_;
	}
private:
	// The table db plugin
	nldb_plugin_table_desc_t meta_table_desc_;
	// The table plugin that has persistent implementation of put/get/del to store meta table records persistently on disk.
	nldb_plugin_leveldb_t meta_table_plugin_;
	// The table context each time we open the meta table.
	nldb_table_context_t meta_table_context_;
	// The database ID
	nldb_db_id_t db_id_;
};

class nldb_tx_impl_t
{
public :
	inline nldb_tx_impl_t() {
		db_      = NULL;
		txEvent_ = NULL;
	}
	inline nldb_rc_t init(nldb_db_impl_t * db) {
		if (this->db_) 
			return NLDB_ERROR_TX_ALREADY_INITIALIZED;
		this->db_ = db;

		return NLDB_OK;
	}
	inline nldb_rc_t begin() {
		// TODO : change to return error.
		tx_assert( ! txEvent_ ); // begin shouldn't be called twice.

		static nldb_tx_id_t nextTxId = INITIAL_TRANSACTION_ID;
		// If the previous tx aborted, we can reuse the current transaction event, as it was not published yet.
		txEvent_ = db_->tx_logs_.getCurrentTransactionEvent();
		if ( txEvent_ ) {
			// There shouldn't be any log records, as all of them were cleared in abort();
			tx_assert( txEvent_->getLogBuffer().getLogRecordCount() == 0 );
		} else {
			txEvent_ = db_->tx_logs_.getNextTransactionEvent();
			tx_assert( txEvent_ );
		}
		// BUGBUG : Do we need a memory barrier here? 
		tx_assert( txEvent_->getLogBuffer().isClear() );

		// BUGBUG : generate txId, set it.
		txEvent_->getLogBuffer().setTransactionId( nextTxId++ );

		return NLDB_OK;
	}

	inline nldb_rc_t commit() {
		nldb_rc_t rc = end();
		if (rc) return rc;

		return NLDB_OK;
	}

	inline nldb_rc_t abort() {
		// BUGBUG : Need to undo any modifying operations on tables.

		// If a transaction aborts, the transaction event in the ring buffer is reused by next transaction.
		// Simply clear the transaction log buffer in case the transaction aborts.
		txEvent_->getLogBuffer().clear();

		// The replication publisher will publish replication message with no log records.
		// This is required for subscribers to check if there is any missing transaction ids or duplicate transaction ids.
		nldb_rc_t rc = end();
		if (rc) return rc;

		return NLDB_OK;
	}

	inline nldb_rc_t append_log_put(const nldb_table_id_t & table_id, const nldb_key_t & key, const nldb_value_t & value) {

		txEvent_->getLogBuffer().appendLog(table_id, TxTransactionLogRedoer::LT_PUT, key.data, key.length, value.data, value.length);

		return NLDB_OK;
	}

	inline nldb_rc_t append_log_del(const nldb_table_id_t & table_id, const nldb_key_t & key) {

		txEvent_->getLogBuffer().appendLog(table_id, TxTransactionLogRedoer::LT_DEL, key.data, key.length, NULL /* value */, 0 /* value length */);

		return NLDB_OK;
	}

	inline nldb_rc_t destroy() {
		if (txEvent_)
			return NLDB_ERROR_TX_NOT_ENDED;
		if (!db_)
			return NLDB_ERROR_TX_ALREADY_DESTROYED;
		db_ = NULL;

		return NLDB_OK;
	}
	inline nldb_db_impl_t * db() {
		return db_;
	}
private :
	// end a transaction so that replication publishers can read the logs.
	inline nldb_rc_t end() {
		tx_assert(txEvent_);

		// Finalize the replication message so that it can be sent to slaves.
		txEvent_->getLogBuffer().finalize();

		// BUGBUG : Think if we need memory barrier here.

		if ( db_->tx_logs_.hasReplicators() )
		{
			// Let the publising thread to publish replication message to slaves.
			db_->tx_logs_.commitCurrentTransactionEvent();
		}
		else
		{
			// There is no replicator at all. Simply clear the log buffer, which is cleared by replicators after they publish the replication message constructed by reading the log buffer.
			txEvent_->getLogBuffer().clear();
		}

		txEvent_ = NULL;

		return NLDB_OK;
	}

	TxTransactionEvent * txEvent_;
	nldb_db_impl_t * db_;
};

class nldb_table_impl_t
{
public :
	nldb_rc_t init(nldb_db_impl_t * db, const nldb_table_id_t & table_id)
	{
		nldb_rc_t rc;
	
		this->db_ = db;

		rc = db_->meta_get(table_id, &table_plugin_id_, &plugin_table_desc_); 
		if ( rc ) return rc;

		table_plugin_ = nldb_plugin(table_plugin_id_);

		rc = table_plugin_->table_open( plugin_table_desc_, & table_context_);
		if ( rc ) return rc;

		table_id_ = table_id;

		return NLDB_OK;
	};

	nldb_rc_t destroy()
	{
		nldb_rc_t rc;

		rc = table_plugin_->table_close( table_context_ );
		if ( rc ) return rc;

		return NLDB_OK;
	}


	inline nldb_rc_t put(const nldb_key_t & key, const nldb_value_t & value)
	{
		nldb_rc_t rc;
		rc = table_plugin_->table_put(table_context_, key, value);
		return rc;
	}

	// errors : NLDB_ERROR_KEY_NOT_FOUND
	inline nldb_rc_t get(const nldb_key_t & key, nldb_value_t * value)
	{
		nldb_rc_t rc;
		rc = table_plugin_->table_get(table_context_, key, value);
		return rc;
	}

	// errors : NLDB_ERROR_KEY_NOT_FOUND
	inline nldb_rc_t del(const nldb_key_t & key)
	{
		nldb_rc_t rc;

		rc = table_plugin_->table_del(table_context_, key);

		return rc;
	}

	// table accessors 
	inline const nldb_table_id_t & id() const
	{
		return table_id_;
	}

	inline const nldb_plugin_id_t & plugin_id() const  {
		return table_plugin_id_;
	}

	inline nldb_plugin_t * table_plugin()
	{
		return table_plugin_;
	}

	inline nldb_table_context_t table_context() {
		return table_context_;
	}

private :
	// The open database where this table exists.
	nldb_db_impl_t  * db_;
	// The id of this table.
	nldb_table_id_t   table_id_;
	// The plugin ID of this table. Based on the plugin, the table's behavior changes. (ex> volitle table, persistent table)
	nldb_plugin_id_t  table_plugin_id_;
	// The plugin that implements open/close/put/get/del ...
	nldb_plugin_t   * table_plugin_;
	// The descriptor of the table that the plugin keeps internal data for maintaining a table. It is also stored persistently in the meta table.
	nldb_plugin_table_desc_t plugin_table_desc_;
	// The table context required to put/get/del a record.
	nldb_table_context_t table_context_;
};

class nldb_cursor_impl_t
{
public :
	nldb_rc_t init(	nldb_db_impl_t * db, nldb_tx_impl_t * tx, nldb_table_impl_t * table, nldb_cursor_context_t cursor_context)
	{
		db_ = db;
		tx_ = tx;
		table_ = table;
		cursor_context_ = cursor_context;

		return NLDB_OK;
	}

	nldb_rc_t destroy()
	{
		// Do nothing
		return NLDB_OK;
	}

	nldb_db_impl_t * db()
	{
		return db_;
	}

	nldb_tx_impl_t * tx()
	{
		return tx_;
	}

	nldb_table_impl_t * table()
	{
		return table_;
	}

	nldb_cursor_context_t & cursor_context()
	{
		return cursor_context_;
	}

private :
	nldb_db_impl_t * db_;
	nldb_tx_impl_t * tx_;
	nldb_table_impl_t * table_;

	nldb_cursor_context_t cursor_context_;
};

/*******************/
/* opaque structs  */
/*******************/

struct nldb_db_holder_t {
	nldb_db_impl_t impl;
};

struct nldb_table_holder_t {
	nldb_table_impl_t impl;
};

struct nldb_tx_holder_t {
	nldb_tx_impl_t impl;
};

struct nldb_cursor_holder_t {
	nldb_cursor_impl_t impl;
};

/*********************/
/* cursor management */
/*********************/
nldb_rc_t nldb_cursor_open(const nldb_tx_t & tx, const nldb_table_t & table, nldb_cursor_t * cursor)
{
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;

	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	nldb_db_impl_t * dbi = txi->db();

	nldb_cursor_holder_t * new_cursor = new nldb_cursor_holder_t;
	if ( ! new_cursor ) return NLDB_ERROR_NO_MEM;

	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) new_cursor;

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_cursor_context_t cursor_context;

	nldb_rc_t rc = table_plugin->cursor_open( tablei->table_context(), &cursor_context);
	if (rc) return rc;

	rc = cursori->init(dbi, txi, tablei, cursor_context);
	if (rc) return rc;

	*cursor = new_cursor;

	return NLDB_OK;
}

// errors : NLDB_CURSOR_NOT_OPEN
nldb_rc_t nldb_cursor_close(const nldb_cursor_t & cursor)
{
	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) cursor;

	nldb_table_impl_t * tablei = cursori->table();

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->cursor_close( tablei->table_context(), cursori->cursor_context() );
	if (rc) return rc;

	rc = cursori->destroy();
	if (rc) return rc;

	nldb_cursor_holder_t * p = cursor;
	delete p;

	return NLDB_OK;
}


nldb_rc_t nldb_cursor_seek_forward(const nldb_cursor_t & cursor, const nldb_key_t & key)
{
	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) cursor;

	nldb_table_impl_t * tablei = cursori->table();

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->cursor_seek_forward(cursori->cursor_context(), key);
	if(rc) return rc;

	return NLDB_OK;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN
nldb_rc_t nldb_cursor_seek_backward(const nldb_cursor_t & cursor, const nldb_key_t & key)
{
	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) cursor;

	nldb_table_impl_t * tablei = cursori->table();

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->cursor_seek_backward(cursori->cursor_context(), key);
	if(rc) return rc;

	return NLDB_OK;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
nldb_rc_t nldb_cursor_move_forward(const nldb_cursor_t & cursor, nldb_key_t * key, nldb_value_t * value)
{
	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) cursor;

	nldb_table_impl_t * tablei = cursori->table();

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->cursor_move_forward(cursori->cursor_context(), key, value);
	if(rc) return rc;

	return NLDB_OK;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
nldb_rc_t nldb_cursor_move_backward(const nldb_cursor_t & cursor, nldb_key_t * key, nldb_value_t * value)
{
	nldb_cursor_impl_t * cursori = (nldb_cursor_impl_t*) cursor;

	nldb_table_impl_t * tablei = cursori->table();

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->cursor_move_backward(cursori->cursor_context(), key, value);
	if(rc) return rc;

	return NLDB_OK;
}





/*****************/
/* db management */
/*****************/
nldb_rc_t nldb_db_create( const nldb_db_id_t & db_id )
{
	tx_assert( db_id < NLDB_MAX_DB_COUNT );
	
	nldb_rc_t rc = nldb_db_impl_t::meta_create(db_id);

	return rc; 
}

nldb_rc_t nldb_db_drop( const nldb_db_id_t & db_id )
{
	tx_assert( db_id < NLDB_MAX_DB_COUNT );

	nldb_rc_t rc = nldb_db_impl_t::meta_drop(db_id);

	return rc;
}

nldb_rc_t nldb_db_open( const nldb_db_id_t & db_id, nldb_replication_master_op_t * master_op, nldb_replication_slave_op_t * slave_op, nldb_db_t * db)
{
	nldb_db_holder_t * new_db = new nldb_db_holder_t;

	if ( ! new_db ) return NLDB_ERROR_NO_MEM;

	nldb_db_impl_t * dbi = (nldb_db_impl_t*) new_db;

	nldb_rc_t rc = dbi->init(db_id);
	if ( rc ) return rc;

	*db = new_db ;

	if ( master_op )
	{
		// Add a replicator which publishes replication messages and clear the log buffer in the ring buffer.
		dbi->add_replicator( dbi->db_id(), master_op->ip, master_op->port );
	}

	if ( slave_op )
	{
		nldb_start_replication_slave_thread( new_db, slave_op->master_desc.ip, slave_op->master_desc.port, slave_op->trigger_handler );

	}

	return NLDB_OK;
}

nldb_rc_t nldb_db_wait_for_replication_publishers(nldb_db_t & db)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	dbi->tx_logs_.waitForReplicationPublishers();

	return NLDB_OK;
}


nldb_rc_t nldb_db_close( nldb_db_t & db)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	nldb_rc_t rc = dbi->destroy();
	if ( rc ) return rc;

	nldb_db_holder_t * p = db;
	delete p;

	return NLDB_OK;
}



// db accessors 
const nldb_db_id_t & nldb_db_id(const nldb_db_t & db)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	return dbi->db_id();
}



/*********************/
/* cursor management */
/*********************/

nldb_rc_t nldb_tx_init(nldb_db_t & db, nldb_tx_t * tx)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t *) db;

	nldb_tx_holder_t * new_tx = new nldb_tx_holder_t;
	if ( ! new_tx ) return NLDB_ERROR_NO_MEM;

	nldb_tx_impl_t * txi = (nldb_tx_impl_t *) new_tx;

	nldb_rc_t rc = txi->init( dbi );
	if (rc) return rc;

	*tx = new_tx;

	return NLDB_OK;
}

// BUGBUG return an error if nldb_tx_init was not called yet.
nldb_rc_t nldb_tx_begin(nldb_tx_t & tx)
{
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;

	return txi->begin();
}

nldb_rc_t nldb_tx_commit(nldb_tx_t & tx)
{
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;

	return txi->commit();
}

nldb_rc_t nldb_tx_abort(nldb_tx_t & tx)
{
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;

	return txi->abort();
}

nldb_rc_t nldb_tx_destroy(nldb_tx_t & tx)
{
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;

	nldb_rc_t rc = txi->destroy();
	if (rc) return rc;

	nldb_tx_holder_t * p = tx;
	delete p;

	return NLDB_OK;
}

/********************/
/* table management */
/********************/
nldb_rc_t nldb_table_create(nldb_db_t & db, const nldb_table_id_t & table_id, const nldb_plugin_id_t & table_plugin_id)
{
	nldb_rc_t rc;

	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	nldb_plugin_t * plugin = nldb_plugin(table_plugin_id);

	nldb_plugin_table_desc_t table_desc;
	
	rc = plugin->table_create(dbi->db_id(), table_id, & table_desc);
	if ( rc ) return rc;
	
	rc = dbi->meta_put(table_id, table_plugin_id, table_desc); 
	if ( rc ) return rc;

	return NLDB_OK;
}

nldb_rc_t nldb_table_drop(nldb_db_t & db, const nldb_table_id_t & table_id)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	return dbi->meta_del(table_id);
}

nldb_rc_t nldb_table_open(nldb_db_t & db, const nldb_table_id_t & table_id, nldb_table_t * table)
{
	nldb_db_impl_t * dbi = (nldb_db_impl_t*) db;

	nldb_table_holder_t * new_table = new nldb_table_holder_t;
	if ( ! new_table ) return NLDB_ERROR_NO_MEM;

	nldb_table_impl_t * tablei = (nldb_table_impl_t*) new_table;

	nldb_rc_t rc = tablei->init(dbi, table_id);
	if (rc) return rc;

	*table = new_table;

	return NLDB_OK;
}

nldb_rc_t nldb_table_close(nldb_table_t & table)
{
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	nldb_rc_t rc = tablei->destroy();
	if (rc) return rc;

	nldb_table_holder_t * p = table;
	delete p;

	return NLDB_OK;
}

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
nldb_rc_t nldb_table_put(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key, const nldb_value_t & value)
{
	nldb_rc_t rc;

	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	rc = tablei->put(key, value);
	if ( rc ) return rc;

	// Append log only if the put operation was successuful.
	// Assume : No need to make sure of WAL, because we we don't write any log to disk, we don't do any restart recovery.
	//          All data structures are kept in memory and they are undone only if the process that embeds LLDB is up and running.
	//          We will undo operations only if the user calls nldb_tx_abort.
	rc = txi->append_log_put(tablei->id(), key, value);
	return rc;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_table_get(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key, nldb_value_t * value)
{
	nldb_rc_t rc;

//	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	rc = tablei->get(key, value);
	return rc;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_table_del(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key)
{
	nldb_rc_t rc;
	
	nldb_tx_impl_t * txi = (nldb_tx_impl_t*) tx;
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;


	rc = tablei->del(key);
	if ( rc ) return rc;

	// Append log only if the put operation was successuful.
	// Assume : Same to nldb_table_put
	rc = txi->append_log_del(tablei->id(), key);
	return rc;
}

// table accessors 
const nldb_table_id_t & nldb_table_id(const nldb_table_t & table)
{
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	return tablei->id();
}

const nldb_plugin_id_t & nldb_table_plugin_id(const nldb_table_t & table)
{
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	return tablei->plugin_id();
}



/*********************/
/* cursor management */
/*********************/
nldb_rc_t nldb_value_free(const nldb_table_t & table, nldb_value_t & value)
{
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->value_free(value);
	if (rc) return rc;

	value.data = NULL;
	value.length = NULL;

	return NLDB_OK;
}

nldb_rc_t nldb_key_free(const nldb_table_t & table, nldb_key_t & key)
{
	nldb_table_impl_t * tablei = (nldb_table_impl_t*) table;

	nldb_plugin_t * table_plugin = tablei->table_plugin();

	nldb_rc_t rc = table_plugin->key_free(key);
	if (rc) return rc;

	key.data = NULL;
	key.length = NULL;

	return NLDB_OK;
}


