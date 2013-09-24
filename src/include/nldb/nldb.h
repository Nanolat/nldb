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

#ifndef _NLDB_H_
#define _NLDB_H_ (1)

#include <nldb/nldb_common.h>
#include <stddef.h>

// When you create a table, you don't specify a string for a table name, but you specify an integer, table id. 
// Why? strings are not efficient data format, using an integer is enough to identify a table.
// The table ID begins from 1 and can have any number up to NLDB_MAX_TABLE_ID.
#define NLDB_MAX_DB_COUNT (1024)
#define NLDB_MAX_TABLE_COUNT_PER_DB ( 65536 )

/******************/
/* opaque structs */
/******************/
typedef struct nldb_db_holder_t     * nldb_db_t;
typedef struct nldb_table_holder_t  * nldb_table_t;
typedef struct nldb_tx_holder_t     * nldb_tx_t;
typedef struct nldb_cursor_holder_t * nldb_cursor_t;

/*********************/
/* plugin management */
/*********************/
extern nldb_plugin_id_t NLDB_TABLE_PERSISTENT;
extern nldb_plugin_id_t NLDB_TABLE_VOLATILE;
extern nldb_plugin_id_t NLDB_TABLE_TC;


// return the maximum number of plugins
extern nldb_uint32_t nldb_plugin_max_count();

/**************************/
/* nldb system management */
/**************************/

extern nldb_rc_t nldb_init();

extern nldb_rc_t nldb_destroy();

/*****************/
/* db management */
/*****************/

extern nldb_rc_t nldb_db_create( const nldb_db_id_t db_id );

extern nldb_rc_t nldb_db_drop( const nldb_db_id_t db_id );

// The options for initializing the instance as replication master. 
// It has a list of replication slave descriptors.
typedef struct nldb_replication_master_op_t {
	// BUGBUG : Make sure that we can replace * to ip address in zmq::socket.bind("tcp://*:5556");
	// The ip of the master.
	char             ip[64];
    // The port that the Master uses to publish transactional logs on the localhost.
	nldb_uint16_t    port;
}nldb_replication_master_op_t;

// The address of replication master. Slaves uses this address to subscribe Master's log publication.
typedef struct nldb_replication_master_desc_t {
	// The ip of the master.
	char             ip[64];
	// The port of the master.
	nldb_uint16_t    port;
} nldb_replication_master_desc_t;

typedef enum nldb_trigger_type_t
{
	TT_PUT = 1 ,
	TT_DEL
} nldb_trigger_type_t ;

typedef nldb_rc_t (*nldb_replication_trigger_hanlder_t)(const nldb_trigger_type_t trigger_type, const nldb_table_id_t table_id, const nldb_key_t & key, const nldb_value_t & value);

// The option for initializing the instance as replication slave.
// It has the port number to open to receive transaction log from replication.
typedef struct nldb_replication_slave_op_t {
	nldb_replication_master_desc_t     master_desc;
	nldb_replication_trigger_hanlder_t trigger_handler;
}nldb_replication_slave_op_t;

// Initialize the lldb instance. an lldb instance has ownership of data replication from all tables in all dbs.
// @master_op : if not NULL, turn on the replication master feature. (sends transaction logs to slaves)
// @slave_op : if not NULL, turn on replication slave feature (sends transaction logs to slaves)
extern nldb_rc_t nldb_db_open( const nldb_db_id_t db_id, nldb_replication_master_op_t * master_op, nldb_replication_slave_op_t * slave_op, nldb_db_t * db);

// Wait for all replication publishers publish all commited transactional replication messages by the time this function was called.
extern nldb_rc_t nldb_db_wait_for_replication_publishers(nldb_db_t & db);


extern nldb_rc_t nldb_db_close( nldb_db_t & db);

// db accessors 
extern nldb_db_id_t nldb_db_id(const nldb_db_t & db);

/**************************/
/* transaction management */
/**************************/

// errors : NLDB_ERROR_TX_ALREADY_INITIALIZED
extern nldb_rc_t nldb_tx_init(nldb_db_t & db, nldb_tx_t * tx);

// errors : NLDB_ERROR_TX_ALREADY_BEGAN
extern nldb_rc_t nldb_tx_begin(nldb_tx_t & tx);

// errors : NLDB_ERROR_TX_NOT_BEGAN

extern nldb_rc_t nldb_tx_commit(nldb_tx_t & tx);

// errors : NLDB_ERROR_TX_NOT_BEGAN
extern nldb_rc_t nldb_tx_abort(nldb_tx_t & tx);

// errors : NLDB_ERROR_TX_NOT_ENDED, NLDB_ERROR_TX_ALREADY_DESTROYED
extern nldb_rc_t nldb_tx_destroy(nldb_tx_t & tx);

/********************/
/* table management */
/********************/

extern nldb_rc_t nldb_table_create(nldb_db_t & db, const nldb_table_id_t table_id, const nldb_plugin_id_t table_plugin_id);

extern nldb_rc_t nldb_table_drop(nldb_db_t & db, const nldb_table_id_t table_id);

extern nldb_rc_t nldb_table_open(nldb_db_t & db, const nldb_table_id_t table_id, nldb_table_t * table);

extern nldb_rc_t nldb_table_close(nldb_table_t & table);

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
extern nldb_rc_t nldb_table_put(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key, const nldb_value_t & value);

// errors : NLDB_ERROR_KEY_NOT_FOUND
extern nldb_rc_t nldb_table_get(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order = NULL);

// errors : NLDB_ERROR_ORDER_OUT_OF_RANGE
extern nldb_rc_t nldb_table_get(nldb_tx_t & tx, nldb_table_t & table, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value);

// errors : NLDB_ERROR_KEY_NOT_FOUND
extern nldb_rc_t nldb_table_del(nldb_tx_t & tx, nldb_table_t & table, const nldb_key_t & key);

extern nldb_rc_t nldb_table_stat(nldb_tx_t & tx, nldb_table_t & table, nldb_table_stat_t * table_stat);


// table accessors 
extern const nldb_table_id_t nldb_table_id(const nldb_table_t & table);
extern const nldb_plugin_id_t nldb_table_plugin_id(const nldb_table_t & table);

/*********************/
/* cursor management */
/*********************/
extern nldb_rc_t nldb_cursor_open(const nldb_tx_t & tx, const nldb_table_t & table, nldb_cursor_t * cursor);

// errors : NLDB_ERROR_CURSOR_NOT_OPEN
extern nldb_rc_t nldb_cursor_seek(const nldb_cursor_t & cursor, const nldb_cursor_direction_t & direction, const nldb_key_t & key);

// errors : NLDB_ERROR_CURSOR_NOT_OPEN
extern nldb_rc_t nldb_cursor_seek(const nldb_cursor_t & cursor, const nldb_cursor_direction_t & direction, const nldb_order_t & order);

// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
extern nldb_rc_t nldb_cursor_fetch(const nldb_cursor_t & cursor, nldb_key_t * key, nldb_value_t * value, nldb_order_t * order = NULL );

// errors : NLDB_CURSOR_NOT_OPEN
extern nldb_rc_t nldb_cursor_close(const nldb_cursor_t & cursor);

/*********************/
/* memory management */
/*********************/
extern nldb_rc_t nldb_value_free(const nldb_table_t & table, nldb_value_t & value);

extern nldb_rc_t nldb_key_free(const nldb_table_t & table, nldb_key_t & key);


#endif
