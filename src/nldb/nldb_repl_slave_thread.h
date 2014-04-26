/*
 * (C) Copyright 2012, 2013 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *                                                      Writen by Kangmo Kim ( kangmo@nanolat.com )
 *
 * =================
 * Apache v2 License
 * =================
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Contributors : 
 *   Kangmo Kim
 */

#ifndef _O_NLDB_REPL_SLAVE_THREAD_H_
#define _O_NLDB_REPL_SLAVE_THREAD_H_ (1)

// Start the replication thread in a slave to receive replication messages from a master to replay transactional logs in them.
void nldb_start_replication_slave_thread(nldb_db_t db, const std::string & master_ip, const unsigned short master_port, const nldb_replication_trigger_hanlder_t & trigger_handler);

#endif /*_O_NLDB_REPL_SLAVE_THREAD_H_*/
