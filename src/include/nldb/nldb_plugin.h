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

#ifndef _NLDB_PLUGIN_H_
#define _NLDB_PLUGIN_H_ (1)

#include <nldb/nldb_common.h>

#define NLDB_MAX_TABLE_DESC_SIZE (256)

// The table description. It is stored in a persistent storage in a db.
typedef struct nldb_plugin_table_desc_t {
	nldb_uint32_t length; // effective length of the table description.
	char          table_desc[NLDB_MAX_TABLE_DESC_SIZE];
} nldb_plugin_table_desc_t;

// The table context for an open table.
typedef void* nldb_table_context_t;

// The table context for an open table cursor.
typedef void* nldb_cursor_context_t;

class nldb_plugin_t
{
public:
	virtual nldb_rc_t table_create(const nldb_table_id_t & db_id, const nldb_table_id_t & table_id, nldb_plugin_table_desc_t * table_desc) = 0;

	virtual nldb_rc_t table_drop(nldb_plugin_table_desc_t & table_desc) = 0;

	virtual nldb_rc_t table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx) = 0;

	virtual nldb_rc_t table_close(nldb_table_context_t table_ctx) = 0;

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
	virtual nldb_rc_t table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value) = 0;

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value) = 0;

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_del(nldb_table_context_t table_ctx, const nldb_key_t & key) = 0;

	// Free allocated memory for the value if any.
	virtual nldb_rc_t value_free(nldb_value_t & value) = 0;

	// Free allocated memory for the key if any.
	virtual nldb_rc_t key_free(nldb_key_t & key) = 0;


	virtual nldb_rc_t cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx) = 0;

	virtual nldb_rc_t cursor_seek_forward (nldb_cursor_context_t cursor_ctx, const nldb_key_t & key) = 0;

	virtual nldb_rc_t cursor_seek_backward(nldb_cursor_context_t cursor_ctx, const nldb_key_t & key) = 0;

	virtual nldb_rc_t cursor_move_forward (nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value) = 0;

	virtual nldb_rc_t cursor_move_backward(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value) = 0;

	virtual nldb_rc_t cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx) = 0;

};

#endif // _NLDB_PLUGIN_H_
