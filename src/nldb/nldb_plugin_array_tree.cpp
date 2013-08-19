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


#include "nldb_plugin_array_tree.h"

#include "nldb_array_tree.h"
#include "nldb_debuggable_array_tree.h"

#include "nldb_object_pool.h"
#include <txbase/tx_assert.h>

nldb_rc_t nldb_plugin_array_tree_t::table_create(const nldb_table_id_t & db_id, const nldb_table_id_t & table_id, nldb_plugin_table_desc_t * table_desc)
{
	// no table descriptor to store.
	table_desc->length = 0;
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::table_drop(nldb_plugin_table_desc_t & table_desc)
{
	// nothing to remove from disk
	return NLDB_OK;
}

const int KEY_SPACE_SIZE = 64;
class table_context_t
{
public :
#if defined(DEBUG)
	typedef nldb_debuggable_array_tree<KEY_SPACE_SIZE> tree_t;
#else
	typedef nldb_array_tree<KEY_SPACE_SIZE> tree_t;
#endif

private :

	tree_t tree_;
	nldb_object_pool * value_pool_;
	bool initialized_;

	// Why don't use key length in tree_ and value length in value_pool_?
	// We will frequently check the key/value size in our plugin, so let's not depend on other modules, which we can't guarantee performance.
	key_length_t key_length_;
	value_length_t value_length_;

public :
	inline const key_length_t & key_length()
	{
		return key_length_;
	}

	inline const value_length_t & value_length()
	{
		return value_length_;
	}

	inline tree_t & tree() {
		return tree_;
	}

	inline nldb_object_pool & value_pool() {
		tx_assert(value_pool_!= NULL);
		return *value_pool_;
	}

	inline bool isInitialized()
	{
		return initialized_;
	}

	table_context_t()
	{
		value_pool_ = NULL;
		initialized_ = false;
		key_length_ = 0;
		value_length_ = 0;
	}

	nldb_rc_t init( const key_length_t & key_length, const value_length_t & value_length )
	{
		tx_assert(!initialized_);

		key_length_ = key_length;
		value_length_ = value_length;

		nldb_rc_t rc = tree_.init(key_length_);
		if (rc)
			return rc;

		value_pool_ = new nldb_object_pool(value_length_);

		initialized_ = true;

		return NLDB_OK;
	}

	nldb_rc_t destroy()
	{
		delete value_pool_;

		nldb_rc_t rc = tree_.destroy();
		if (rc)
			return rc;

		return NLDB_OK;
	}
};

nldb_rc_t nldb_plugin_array_tree_t::table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx)
{
	table_context_t * ctx = new table_context_t();
	*table_ctx = ctx;
	return NLDB_OK;
}


#if defined(DEBUG)
bool __print_nodes_on_table_close = false;
bool __check_consistency_on_table_close = false;
// Print all keys in all nodes in ascending order.
nldb_rc_t debug_print_nodes(nldb_table_context_t table_ctx)
{
	table_context_t * ctx = (table_context_t*) table_ctx;

	table_context_t::tree_t & tree = ctx->tree();

	int print_iteration = tree.get_print_iteration();

	(void)tree.print_tree(print_iteration);

	return NLDB_OK;
}

nldb_rc_t debug_check_consistency(nldb_table_context_t table_ctx)
{
	table_context_t * ctx = (table_context_t*) table_ctx;

	table_context_t::tree_t & tree = ctx->tree();

	int print_iteration = tree.get_print_iteration();

	(void) tree.check_consistency(print_iteration);

	return NLDB_OK;
}
#endif // DEBUG

nldb_rc_t nldb_plugin_array_tree_t::table_close(nldb_table_context_t table_ctx)
{
	table_context_t * ctx = (table_context_t*) table_ctx;

#if defined(DEBUG)
	if (__print_nodes_on_table_close)
	{
		tx_assert( debug_print_nodes(ctx) == NLDB_OK );
		__print_nodes_on_table_close = false;
	}

	if (__check_consistency_on_table_close)
	{
		tx_assert( debug_check_consistency(ctx) == NLDB_OK );
		__check_consistency_on_table_close = false;
	}
#endif // DEBUG

	if (ctx->isInitialized())
	{
		ctx->destroy();
	}

	delete ctx;

	return NLDB_OK;
}

static inline nldb_rc_t check_context_initialized(table_context_t * ctx, const key_length_t & key_length, const value_length_t & value_length)
{
	if (ctx->isInitialized())
	{
		if ( ctx->key_length() != key_length )
		{
			return NLDB_ERROR_VARIABLE_KEY_SIZE_NOT_SUPPORTED;
		}
		if ( ctx->value_length() != value_length)
		{
			return NLDB_ERROR_VARIABLE_VALUE_SIZE_NOT_SUPPORTED;
		}
	}
	else
	{
		// Initialize the tree.
		nldb_rc_t rc = ctx->init(key_length, value_length);
		if (rc)
			return rc;
	}

	return NLDB_OK;
}

static inline nldb_rc_t check_key_length(table_context_t * ctx, const key_length_t & key_length)
{
	if ( ctx->key_length() != key_length )
	{
		return NLDB_ERROR_VARIABLE_KEY_SIZE_NOT_SUPPORTED;
	}
	return NLDB_OK;
}

static inline nldb_rc_t check_value_length(table_context_t * ctx, const value_length_t & value_length)
{
	if ( ctx->value_length() != value_length)
	{
		return NLDB_ERROR_VARIABLE_VALUE_SIZE_NOT_SUPPORTED;
	}
	return NLDB_OK;
}
// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
nldb_rc_t nldb_plugin_array_tree_t::table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value)
{
	nldb_rc_t rc = NLDB_OK;

	table_context_t * ctx = (table_context_t*) table_ctx;

	tx_assert( check_context_initialized(ctx, key.length, value.length) == NLDB_OK );

	void * value_ptr = ctx->value_pool().malloc();
	memcpy(value_ptr, value.data, value.length);

	rc = ctx->tree().put(key.data, value_ptr);
	if (rc) return rc;

	return NLDB_OK;
}


// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_array_tree_t::table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order)
{
	nldb_rc_t rc = NLDB_OK;

	table_context_t * ctx = (table_context_t*) table_ctx;

	if ( ! ctx->isInitialized() )
	{
		// The table context was not initialized. It means there was no trial to put any key.
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	// BUGBUG -- implement it!
	if (order != NULL)
		return NLDB_ERROR_UNSUPPORTED_FEATURE;

	void * value_ptr = NULL;
	rc = ctx->tree().get(key.data, &value_ptr);
	if (rc) return rc;

	if (value_ptr )
	{
		value->length = ctx->value_length();
		value->data = value_ptr;
	}
	else
	{
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	return NLDB_OK;
}

// errors : NLDB_ERROR_ORDER_OUT_OF_RANGE
nldb_rc_t nldb_plugin_array_tree_t::table_get(nldb_table_context_t table_ctx, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value)
{
	// BUGBUG -- implement it!
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_array_tree_t::table_del(nldb_table_context_t table_ctx, const nldb_key_t & key)
{
	nldb_rc_t rc = NLDB_OK;

	table_context_t * ctx = (table_context_t*) table_ctx;

	if ( ! ctx->isInitialized() )
	{
		// The table context was not initialized. It means there was no trial to put any key.
		// TODO : Make sure that we need to return NLDB_ERROR_KEY_NOT_FOUND in case there is no such key.
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	void * value_ptr;
	rc = ctx->tree().del(key.data, &value_ptr);
	if (rc) return rc;

	if ( value_ptr )
	{
		// return the value ptr to the value pool if the key was found.
		ctx->value_pool().free(value_ptr);
	}
	else
	{
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat)
{
	// BUGBUG -- implement it!
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

nldb_rc_t nldb_plugin_array_tree_t::value_free(nldb_value_t & value) {
	// This function is called to free values that are returned by the plugin interface.
	// We don't allocate any new memory for the values, but we simply return pointer to the value.
	// So, we SHOULD NOT free the memory used by values.
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::key_free(nldb_key_t & key) {
	// Same to value_free, we SHOULD NOT free the memory used by keys.
	return NLDB_OK;
}


class cursor_context_t
{
public :
	typedef enum direction {
		SEEK_FORWARD = 1,
		SEEK_BACKWARD = 2
	} direction;

	table_context_t::tree_t::iterator_t iter_;
	table_context_t * table_ctx_;

	direction dir_;

	cursor_context_t(table_context_t * table_ctx)
	{
		table_ctx_ = table_ctx;
	}
	virtual ~cursor_context_t()
	{
	}
};

nldb_rc_t nldb_plugin_array_tree_t::cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx)
{
	table_context_t * the_table_context = (table_context_t*)table_ctx ;

	cursor_context_t * new_cursor_ctx = new cursor_context_t(the_table_context);

	*cursor_ctx = new_cursor_ctx;

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::cursor_seek_forward (nldb_cursor_context_t cursor_ctx, const nldb_key_t & key)
{
	cursor_context_t* the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		nldb_rc_t rc = check_key_length(the_table_ctx, key.length);
		if (rc) return rc;

		rc = the_table_ctx->tree().seek_forward( key.data, & the_cursor_ctx->iter_);
		if (rc) return rc;

		the_cursor_ctx->dir_ = cursor_context_t::SEEK_FORWARD;
	}
	else
	{
		// No nothing, just return NLDB_OK.
		// cursor_move_forward will return NLDB_ERROR_END_OF_ITERATION.
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::cursor_move_forward (nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value)
{
	cursor_context_t * the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	// TODO : check if cursor_seek_forward was invoked.

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		// make sure that cursor_seek_forward was invoked.
		if ( the_cursor_ctx->dir_ != cursor_context_t::SEEK_FORWARD )
		{
			return NLDB_ERROR_INVALID_ITERATION;
		}

		bool end_of_iteration = false;
		void * key_data = NULL;
		void * value_data = NULL;
		nldb_rc_t rc = the_table_ctx->tree().move_forward( the_cursor_ctx->iter_, & key_data, & value_data, & end_of_iteration);
		if (rc) return rc;

		if (end_of_iteration)
			return NLDB_ERROR_END_OF_ITERATION;

		key->length = the_table_ctx->key_length();
		key->data = key_data;

		value->length = the_table_ctx->value_length();
		value->data = value_data;
	}
	else
	{
		return NLDB_ERROR_END_OF_ITERATION;
	}
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::cursor_seek_backward(nldb_cursor_context_t cursor_ctx, const nldb_key_t & key)
{
	cursor_context_t* the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		nldb_rc_t rc = check_key_length(the_table_ctx, key.length);
		if (rc) return rc;

		rc = the_table_ctx->tree().seek_backward( key.data, & the_cursor_ctx->iter_);
		if (rc) return rc;

		the_cursor_ctx->dir_ = cursor_context_t::SEEK_BACKWARD;
	}
	else
	{
		// No nothing, just return NLDB_OK.
		// cursor_move_forward will return NLDB_ERROR_END_OF_ITERATION.
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::cursor_move_backward(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value)
{
	cursor_context_t * the_cursor_ctx = (cursor_context_t*) cursor_ctx;


	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		// make sure that cursor_seek_backward was invoked.
		if ( the_cursor_ctx->dir_ != cursor_context_t::SEEK_BACKWARD )
		{
			return NLDB_ERROR_INVALID_ITERATION;
		}

		bool end_of_iteration = false;
		void * key_data = NULL;
		void * value_data = NULL;
		nldb_rc_t rc = the_table_ctx->tree().move_backward( the_cursor_ctx->iter_, & key_data, & value_data, & end_of_iteration);
		if (rc) return rc;

		if (end_of_iteration)
			return NLDB_ERROR_END_OF_ITERATION;

		key->length = the_table_ctx->key_length();
		key->data = key_data;

		value->length = the_table_ctx->value_length();
		value->data = value_data;
	}
	else
	{
		return NLDB_ERROR_END_OF_ITERATION;
	}
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx)
{
	cursor_context_t* the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	delete the_cursor_ctx;

	return NLDB_OK;
}
