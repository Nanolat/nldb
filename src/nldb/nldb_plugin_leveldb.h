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

#ifndef _NLDB_PLUGIN_LEVELDB_H_
#define _NLDB_PLUGIN_LEVELDB_H_ (1)

#include <nldb/nldb_plugin.h>

class nldb_plugin_leveldb_t : public nldb_plugin_t
{
public:
	virtual ~nldb_plugin_leveldb_t() {};

	virtual nldb_rc_t table_create(const nldb_table_id_t db_id, const nldb_table_id_t table_id, nldb_plugin_table_desc_t * table_desc);

	virtual nldb_rc_t table_drop(nldb_plugin_table_desc_t & table_desc);

	virtual nldb_rc_t table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx );

	virtual nldb_rc_t table_close(nldb_table_context_t table_ctx );

	virtual nldb_rc_t table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value);

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order);

// errors : NLDB_ORDER_OUT_OF_RANGE
	virtual nldb_rc_t table_get(nldb_table_context_t table_ctx, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value );

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_del(nldb_table_context_t table_ctx, const nldb_key_t & key);

	virtual nldb_rc_t table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat);

	// Free allocated memory for the value if any.
	virtual nldb_rc_t value_free(nldb_value_t & value);

	// Free allocated memory for the key if any.
	virtual nldb_rc_t key_free(nldb_key_t & key);

	virtual nldb_rc_t cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx);

	virtual nldb_rc_t cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_key_t & key);

	// errors : NLDB_ERROR_CURSOR_NOT_OPEN
	virtual nldb_rc_t cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_order_t & order);

	// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
	virtual nldb_rc_t cursor_fetch(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value, nldb_order_t * order);

	virtual nldb_rc_t cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx);
};

// The descriptor for the meta table that keeps the list of tables.
extern nldb_plugin_table_desc_t nldb_meta_table_desc(const nldb_db_id_t db_id);

// Check if a table exists.
extern nldb_rc_t nldb_table_exists(nldb_plugin_table_desc_t & table_desc, bool * exists);


#endif // _NLDB_PLUGIN_LEVELDB_H_
