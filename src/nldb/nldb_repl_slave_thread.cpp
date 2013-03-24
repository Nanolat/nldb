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

//
//  TxReplicationServiceThread.cpp
//  Tilemap
//
//  Created by Kangmo Kim on 12. 8. 9..
//
//

#include <txbase/tx_assert.h>

#include <thread>
#include <xs/xs.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include "nldb_transaction_log_buffer.h"
#include "nldb_transaction_log_redoer.h"

/*
// The log replayer that receives replication messages from master to apply transactional logs to tables in slaves.
class LogReplayer 
{  
public:   
    LogReplayer(nldb_db_t & db, const std::string & master_ip, const unsigned short master_port)
            : log_redoer_(db), 
			  master_ip_(master_ip),  
              master_port_(master_port)
    {  
    } 
	~LogReplayer()
	{
	}
       
   
private:  
    LogReplayer(); // no default constructor available.

    std::string master_ip_;
	unsigned short master_port_;

	// Responsible for redoing log records within a replication message.
	TxTransactionLogRedoer log_redoer_;
};  
*/

void replication_slave_thread_func(nldb_db_t & db, const std::string & master_ip, const unsigned short master_port, const nldb_replication_trigger_hanlder_t & trigger_handler)
{  
	std::shared_ptr<TxTransactionLogRedoer> logRedoer( new TxTransactionLogRedoer(db, trigger_handler) );

	void * context = xs_init();
	tx_assert(context);
 
	void * subscriber = xs_socket(context, XS_SUB);
	tx_assert(subscriber);



	// To subscribe the topic, set the filter here. We will receive messages that start with the topic string only.
	//const char *filter = "A TOPIC ";
	// To receive all messages without specifying any topic, just set it as an empty string.
	const char *filter = "";
	int xrc = xs_setsockopt(subscriber, XS_SUBSCRIBE, filter, strlen (filter));
	tx_assert(xrc == 0);

/*
	// Use the dbid as an identity of the PUB-SUB sessions, even though the process restarts, we will receive messages from master without losing any of them.
	nldb_db_id_t dbid = nldb_db_id(db);
	xrc = xs_setsockopt(subscriber, XS_IDENTITY, &dbid, sizeof(dbid));
	tx_assert(xrc == 0);
*/
/*
	int newbufsize = 1024L * 1024L * 1024L; // The SO_RCVBUF buffer size on the underlying socket
	xrc = xs_setsockopt(subscriber, XS_RCVBUF, &newbufsize, sizeof(newbufsize));
	tx_assert(xrc == 0);
*/

	int newhwm = 1024L * 1024L * 1024L; // The number of messages.
	xrc = xs_setsockopt(subscriber, XS_RCVHWM, &newhwm, sizeof(newhwm));
	tx_assert(xrc == 0);

	int hwm;
	size_t hwmsize = sizeof(hwm); // The number of messages.
	xrc = xs_getsockopt(subscriber, XS_RCVHWM, &hwm, &hwmsize);
	tx_assert(xrc == 0);
	printf("[XS_RCVHWM:%d]", hwm);

	// The master address required to subscribe messages from it.
	char master_addr[512];
	master_addr[sizeof(master_addr)-1] = 0;
	_snprintf(master_addr, sizeof(master_addr), "tcp://%s:%d", master_ip.c_str(), master_port );
	// make sure that we didn't hit the end of buffer
	tx_assert(master_addr[sizeof(master_addr)-1] == 0);

	xrc = xs_connect(subscriber,  master_addr );
	tx_assert(xrc != -1);


	nldb_rc_t rc;
	rc = logRedoer->init();
	if (rc) {
		// BUGBUG : Change the code to write error logs.
		printf("[Slave:LogReplayer] failed to initialize log redoer. error code : %d", rc);
		return;
	}

	printf("[Slave:LogReplayer] waiting for tranasctional replication log messages from master.\n");

	size_t receivedMessageCount = 0;
	// BUGBUG : Need a flag to stop the log replayer?
	for (;;) {
		xs_msg_t replicationMessage;

		xrc = xs_msg_init ( &replicationMessage );
		tx_assert (xrc == 0);

		xrc = xs_recvmsg(subscriber, &replicationMessage, 0);
		tx_assert(xrc != -1);

		receivedMessageCount ++;

		if ((receivedMessageCount & 0xFFFFF) == 0 )
		{
			printf("Log replayer (message count = %lu): received a replication message from the master.\n", receivedMessageCount);
		}

		rc = TxTransactionLogBuffer::applyReplicationMessage(& replicationMessage, *logRedoer);
		if (rc) {
			// BUGBUG : Change the code to write error logs.
			printf("[Slave:LogReplayer] failed to apply replication message. error code : %d", rc);
		}
		// applyReplicationMessage should always succeed.
		tx_assert(rc == 0);

		xrc = xs_msg_close ( &replicationMessage );
		tx_assert (xrc == 0);
	}

	xrc = xs_close (subscriber);
	tx_assert(xrc == 0);

	xrc = xs_term (context);
	tx_assert(xrc == 0);

	// BUGBUG : when this thread exits, we need to destroy log_redoer_. 
	// Currently we don't support "shutdown" of a LLDB process, so we don't implement it.
}  


void nldb_start_replication_slave_thread(nldb_db_t & db, const std::string & master_ip, const unsigned short master_port, const nldb_replication_trigger_hanlder_t & trigger_handler)
{
    std::thread serviceThread(replication_slave_thread_func, db, master_ip, master_port, trigger_handler);

	// BUGBUG : while shutdown of the process, we need to join the slave replicator thread.
}

