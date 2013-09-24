/*
 * (C) Copyright 2012, 2013 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *                                                      Writen by Kangmo Kim ( kangmo@nanolat.com )
 *
 * ============
 * Dual License
 * ============
 * You can choose either free(AGPL v3) or commercial( Similar to Apache license v2 ).
 * 
 * ================================================================
 * Nanolat Database Commercial License for OEMs, ISVs, VARs and SPs
 * ================================================================
 * Nanolat provides its Nanolat Database server and Nanolat Client Libraries under a dual license model
 * designed to meet the development and distribution needs of both commercial distributors / service providers
 * (such as OEMs, ISVs, VARs, and SPs) and open source projects.
 *
 * ==============================================================================
 * For OEMs, ISVs, VARs and Other Distributors of Commercial Applications and SPs
 * ==============================================================================
 * OEMs (Original Equipment Manufacturers), ISVs (Independent Software Vendors), VARs (Value Added Resellers)
 * and other distributors that combine and distribute commercially licensed software
 * with Nanolat Database software but do not wish to distribute the source code for the commercially licensed software
 * under version 3 of the GNU Affero General Public License (the "AGPL") must enter into a commercial license agreement with Nanolat.
 * SPs (Service Providers) also need to enter into a commercial license agreement with Nanolat otherwise they are under the AGPL v3.
 * The commercial license includes Apache v2 license, which does not require you to open source the project developed using Nanolat Database.
 * 
 * For start-up companies, Nanolat provides commercial license and technical support at small percentage of equity share.
 * Focus on your client side development and grow your business quicker by using Nanolat Database.
 * Send an email to support@nanolat.com for the details.
 *
 * =========================================================================
 * For Open Source Projects and Other Developers of Open Source Applications
 * =========================================================================
 * For developers of Free Open Source Software ("FOSS") applications under the GPL v3 or AGPL v3
 * that want to combine and distribute those FOSS applications with Nanolat Database software,
 * Nanolat Database open source software licensed under the AGPL v3 is the best option.
 */


#ifndef _O_NLDB_TRANSCTION_LOG_REDOER_H_
#define _O_NLDB_TRANSCTION_LOG_REDOER_H_ (1)

#include <txbase/tx_assert.h>

#include <nldb/nldb.h>
#include "nldb_transaction_log_buffer.h"

// Redo transaction logs
// For each replication message :
//    begin a transaction, open tables if not open, redo log records, commit a transaction
//    assumption : open tables still remain open even though the transaction commits.
// 
// (P2) Shutdown : Need decision - do we need shutdown? how about simply kill the process and do restart recovery always ?
//    close all open tables
class TxTransactionLogRedoer {
public :
	typedef enum log_type_t {
		LT_PUT = 1,
		LT_DEL
	}log_type_t;

	typedef struct table_t {
		nldb_table_t table;
		// indicates if this table is open
		bool is_open_;
	} table_t ;
	
	TxTransactionLogRedoer(nldb_db_t & db, const nldb_replication_trigger_hanlder_t & trigger_handler) : db_(db) {
		for (unsigned int i=0; i < sizeof(tables_)/sizeof(tables_[0]); i++ ) {
			tables_[i].is_open_ = false;
		}

		trigger_handler_ = trigger_handler;
	}

	nldb_rc_t init() {
		nldb_rc_t
		rc = nldb_tx_init( db_, &redoingTx );
		return rc;
	}

	// Not used for now.
	/*
	nldb_rc_t destroy() {
		nldb_rc_t
		rc = nldb_tx_destroy( redoingTx );
		return rc;
	}
	*/

	// @param masterTxId : the transaciotn Id at master. Not used for now.
	nldb_rc_t beginTransaction(const nldb_tx_id_t & masterTxId)
	{
		nldb_rc_t rc = nldb_tx_begin(redoingTx);
		return rc;
	}

	nldb_rc_t redoLog(const nldb_table_id_t & tableId, const log_type_t & logType, void * keyData, key_length_t keyLength, void * valueData, value_length_t valueLength)
	{
		nldb_table_t * table = NULL;
		nldb_rc_t rc = getOpenTable( tableId, & table );

		nldb_key_t nldb_key = { keyData, keyLength };

		switch (logType) {
			case LT_PUT :
			{
				nldb_value_t nldb_value = { valueData, sizeof(valueLength) };

				rc = nldb_table_put( redoingTx, *table, nldb_key, nldb_value);
				if (rc) return rc;

				if (trigger_handler_)
					trigger_handler_(TT_PUT, tableId, nldb_key, nldb_value);

			}
			break;
			case LT_DEL :
			{
				rc = nldb_table_del( redoingTx, *table, nldb_key);
				if (rc) return rc;
				if (trigger_handler_)
				{
					nldb_value_t nldb_value = { NULL, 0 };
					trigger_handler_(TT_DEL, tableId, nldb_key, nldb_value);
				}
			}
			break;
			default :
			{
				// Above log types are the only ones that we know.
				tx_assert(0);
			}
		}
		return NLDB_OK;
	}

	// @param masterTxId : the transaciotn Id at master. Not used for now.
	nldb_rc_t commitTransaction(const nldb_tx_id_t & masterTxId)
	{
		nldb_rc_t rc = nldb_tx_commit(redoingTx);
		return rc;
	}

	// @param masterTxId : the transaciotn Id at master. Not used for now.
	nldb_rc_t abortTransaction(const nldb_tx_id_t & masterTxId)
	{
		nldb_rc_t rc = nldb_tx_abort(redoingTx);
		return rc;
	}

private :
	// Set the output parameter table to the handle of open table. If the table is not open yet, this function opens it first.
	nldb_rc_t getOpenTable(const nldb_table_id_t & tableId, nldb_table_t ** table )
	{
		tx_assert( tableId <= NLDB_MAX_TABLE_COUNT_PER_DB );
		
		nldb_table_t * theTable;

		theTable = & tables_[tableId].table;

		if ( ! tables_[tableId].is_open_ ) {
			nldb_rc_t rc = nldb_table_open(db_, tableId, theTable );
			if (rc) return rc;
			tables_[tableId].is_open_ = true;
		}

		*table = theTable;
		return NLDB_OK;
	};

	// The database where we redo log records.
	nldb_db_t & db_;

	// The tranasction id assigned by the time slave begins a transaction.
	nldb_tx_t redoingTx;

	// use table id as an index to this array. table id starts from 1.
	table_t tables_[NLDB_MAX_TABLE_COUNT_PER_DB + 1];

	// The handler function that is called whenever PUT/DEL operation is replicated.
	nldb_replication_trigger_hanlder_t trigger_handler_;
};

#endif /* _O_NLDB_TRANSCTION_LOG_REDOER_H_ */
