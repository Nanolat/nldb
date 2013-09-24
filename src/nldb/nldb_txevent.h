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


#ifndef _O_TASKDEF_H_ // NOLINT
#define _O_TASKDEF_H_ // NOLINT

#include <txbase/tx_assert.h>
#include <assert.h>

#if defined(_MSC_VER)
#  include <boost/atomic.hpp>
#else
#  include <atomic>
#endif

#include <string>

#include <disruptor/interface.h>

#include <xs/xs.h>

#include <nldb/nldb_common.h>

#include "nldb_transaction_log_buffer.h"

#include "nldb_internal.h"

class LongEvent {
 public:
    LongEvent(const int64_t& value = 0) : value_(value) {}

    int64_t value() const { return value_; }

    void set_value(const int64_t& value) { value_ = value; }

 private:
    int64_t value_;
};

class TxTransactionEvent : public LongEvent {
 public:

    TxTransactionEvent(const int64_t& value = 0) : LongEvent(value)  {
	}

    TxTransactionLogBuffer & getLogBuffer() {
		return logBuffer_;
	}

private:
	TxTransactionLogBuffer logBuffer_;
};

class TxTransactionEventFactory : public disruptor::EventFactoryInterface<TxTransactionEvent> {
 public:
	TxTransactionEventFactory() 
	{
	};
	virtual ~TxTransactionEventFactory() {};

    virtual TxTransactionEvent* NewInstance(const int& size) const {
        return new TxTransactionEvent[size];
    }
};


class TxTransactionLogPublisher : public disruptor::EventHandlerInterface<TxTransactionEvent> {
 public:
    TxTransactionLogPublisher(
    	const nldb_db_id_t dbid,
    	const std::string & master_ip,
    	const unsigned short master_port)
		: master_ip_(master_ip), master_port_(master_port)  {
		


		char bind_addr[512];
		bind_addr[sizeof(bind_addr)-1] = 0;
		_snprintf(bind_addr, sizeof(bind_addr), "tcp://%s:%d", master_ip.c_str(), master_port );
		// make sure that // didn't hit the end of buffer.
		tx_assert(bind_addr[sizeof(bind_addr)-1] == 0);


		context_ = xs_init ();
		tx_assert(context_);

		publisher_ = xs_socket (context_, XS_PUB);
		tx_assert(publisher_);

		int newhwm = 1024L * 1024L * 1024L; // The number of messages.
		int rc = xs_setsockopt(publisher_, XS_SNDHWM, &newhwm, sizeof(newhwm));
		tx_assert(rc == 0);

		int hwm;
		size_t hwmsize = sizeof(hwm); // The number of messages.
		rc = xs_getsockopt(publisher_, XS_SNDHWM, &hwm, &hwmsize);
		tx_assert(rc == 0);
		printf("[XS_SNDHWM:%d]", hwm);
/*
		int newbufsize = 1024L * 1024L * 1024L; // The SO_SNDBUF buffer size on the underlying socket
		rc = xs_setsockopt(publisher_, XS_SNDBUF, &newbufsize, sizeof(newbufsize));
		tx_assert(rc == 0);
*/
/*
		// Use the dbid as an identity of the PUB-SUB sessions, even though the subscribing process restarts, they will receive messages from master without losing any of them.
		rc = xs_setsockopt(publisher_, XS_IDENTITY, &dbid, sizeof(dbid));
		tx_assert( rc == 0 );
*/
		// TODO : Return an error code instead of assertion.
		rc = xs_bind (publisher_, bind_addr);
		tx_assert (rc != -1);



/*		
		// BUGBUG : Need to set HWM to a enough size, and set HWM option to Block.
		// Currently we set HWM to unlimitted(0), this means we may end of with not enough memory error when subscribers are down.
		int64_t hwm = 0;
		publisher.setsockopt (ZMQ_HWM, &hwm, sizeof(hwm));
*/
		// Sleep for 5 seconds to connect to subscribers. Otherwise we can lose some messages at the starting point, because it takes time for the ZMQ publisher and subscribers to connect to each other.
		// BUGBUG : Find out how we can explic= ity check if the publisher and subscriber are connected together.
    std::this_thread::sleep_for(std::chrono::milliseconds(XS_HANDSHAKE_SLEEP_SECONDS * 10));
	}
	
	virtual ~TxTransactionLogPublisher() {
		int rc = xs_close (publisher_);
		tx_assert(rc == 0);

		rc = xs_term (context_);
		tx_assert(rc == 0);

		publisher_ = NULL;
		context_ = NULL;
	};

    virtual void OnEvent(const int64_t& sequence,
                         const bool& end_of_batch,
                         TxTransactionEvent* event) {

        if (event)
            event->set_value(sequence);

		// Should be finalized to publish the replication message to slaves.
		tx_assert( event->getLogBuffer().isFinalized() );

		// Assume : The transaction id in the replication message monotonously increases. Aborted transactions are also published not to have any missing transaction id.
		static nldb_tx_id_t prevTransactionId = INITIAL_TRANSACTION_ID - 1;

		// Make sure that the transaction id increases monotonously without any missing id, nor duplicate id.
		nldb_tx_id_t currentTransactionId = event->getLogBuffer().getTransactionId();
		if ( prevTransactionId + 1 != currentTransactionId) 
		{
			printf("[Replication Publisher] Transaction Id Mismatch - Expected : %lld, Actual : %lld\n", prevTransactionId + 1,  currentTransactionId);
			tx_assert(0);
		}
/*
		if ( (prevTransactionId & 0xFF) == 0 ) {
			boost::this_thread::sleep(boost::posix_time::microseconds(35));
		}
*/
		prevTransactionId = currentTransactionId;

		xs_msg_t replicationMessage;

		event->getLogBuffer().buildReplicationMessage( & replicationMessage );

		TxTransactionLogBuffer::checkReplicationMessage( & replicationMessage );
		
		int rc = xs_sendmsg(publisher_, &replicationMessage, 0);
		tx_assert(rc);

		// We are done. Reset the log buffer before other transactions use it.
		event->getLogBuffer().reset();

		if ((sequence & 0xFFFFF) == 0 )
		{
			printf("Log publisher (sequence = %lld): published replication messages to slaves.\n",
					(unsigned long long int)sequence);
		}

		event->getLogBuffer().destroyReplicationMessage(&replicationMessage);
    };

    virtual void OnStart() {}
    virtual void OnShutdown() {}
 private :
	std::string master_ip_;
	unsigned short master_port_;
    void *context_;
    void *publisher_;
};

class TxTransactionEventTranslator : public disruptor::EventTranslatorInterface<TxTransactionEvent> {
 public:
	 virtual ~TxTransactionEventTranslator() {};
    virtual TxTransactionEvent* TranslateTo(const int64_t& sequence, TxTransactionEvent* event) {
        event->set_value(sequence);

		return event;
    };
};

#endif //_O_TASKDEF_H_
