/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Kangmo Kim
 *     Saryong Kang
 */

#ifndef _O_NLDB_REPL_SLAVE_THREAD_H_
#define _O_NLDB_REPL_SLAVE_THREAD_H_ (1)

// Start the replication thread in a slave to receive replication messages from a master to replay transactional logs in them.
void nldb_start_replication_slave_thread(nldb_db_t & db, const std::string & master_ip, const unsigned short master_port, const nldb_replication_trigger_hanlder_t & trigger_handler);

#endif /*_O_NLDB_REPL_SLAVE_THREAD_H_*/
