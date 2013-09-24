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


#include "nldb_plugin_array_tree.h"

#include "nldb_array_tree.h"
#include "nldb_debuggable_array_tree.h"

#include "nldb_object_pool.h"
#include <txbase/tx_assert.h>

const int NLDB_VALUE_TAIL_MAGIC = 0xcafe7777;

nldb_rc_t nldb_plugin_array_tree_t::table_create(const nldb_table_id_t db_id, const nldb_table_id_t table_id, nldb_plugin_table_desc_t * table_desc)
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

class table_context_t
{
public :
#if defined(NDEBUG)
#define KEY_SPACE_SIZE (64)
	typedef nldb_array_tree<KEY_SPACE_SIZE> tree_t;
#else
	/* We can set the key space size to 24, to make node split happen more frequently and make the depth of the tree deeper */
#define KEY_SPACE_SIZE (64)
	typedef nldb_debuggable_array_tree<KEY_SPACE_SIZE> tree_t;
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
	}

	nldb_rc_t init( const key_length_t & key_length )
	{
		tx_assert(!initialized_);

		key_length_ = key_length;

		nldb_rc_t rc = tree_.init(key_length_);
		if (rc)
			return rc;

		value_pool_ = new nldb_object_pool();

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

static inline nldb_rc_t check_context_initialized(table_context_t * ctx, const key_length_t & key_length)
{
	if (ctx->isInitialized())
	{
		if ( ctx->key_length() != key_length )
		{
			printf("Different key size detected. original:%d, new:%d\n", ctx->key_length() , key_length );
			return NLDB_ERROR_VARIABLE_KEY_SIZE_NOT_SUPPORTED;
		}
	}
	else
	{
		// Initialize the tree.
		nldb_rc_t rc = ctx->init(key_length);
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

inline void pack_value_object(nldb_object_pool * pool, value_length_t value_length, void * value, void **o_packed_value_object) {
	tx_debug_assert(pool);
	tx_debug_assert(value);
	tx_debug_assert(o_packed_value_object);

	// These sizes are from length, actual value, magic.
	int value_object_size = sizeof(value_length_t) + value_length + sizeof(int);

	char * value_object_ptr = (char *) pool->malloc(value_object_size);
	// set the length.
	*((value_length_t*) value_object_ptr) = value_length;
	// copy value.
	memcpy( value_object_ptr+sizeof(value_length_t), value, value_length);
	// set magic of value tail.
	*( (int*) (value_object_ptr + sizeof(value_length_t) + value_length) ) = NLDB_VALUE_TAIL_MAGIC;

	*o_packed_value_object = value_object_ptr;
}

inline void unpack_value_object(void * packed_value_object, value_length_t * o_value_length, void ** o_value) {
	tx_debug_assert(packed_value_object);
	tx_debug_assert(o_value_length);
	tx_debug_assert(o_value);

	char * p = (char *)packed_value_object;

	value_length_t value_length = *((value_length_t*)p);
	tx_assert(value_length > 0);

	char * value_ptr = p + sizeof(value_length_t);

	int magic = *( (int*) (value_ptr + value_length) );
	assert( magic == NLDB_VALUE_TAIL_MAGIC );

	*o_value_length = value_length;
	*o_value = (void*)value_ptr;
}

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
nldb_rc_t nldb_plugin_array_tree_t::table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value)
{
	nldb_rc_t rc = NLDB_OK;

	table_context_t * ctx = (table_context_t*) table_ctx;

	tx_assert( check_context_initialized(ctx, key.length) == NLDB_OK );

	void * packed_value_ptr;
	pack_value_object(&ctx->value_pool(), value.length, value.data, &packed_value_ptr);

	rc = ctx->tree().put(key.data, packed_value_ptr);
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

	void * packed_value_ptr = NULL;
	// order is passed to tree().get function. it assigns the order of the key to *order only if order is not NULL.
	rc = ctx->tree().get(key.data, &packed_value_ptr, order);
	if (rc) return rc;

	if (packed_value_ptr )
	{
		unpack_value_object(packed_value_ptr, &value->length, &value->data);
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
	nldb_rc_t rc = NLDB_OK;

	table_context_t * ctx = (table_context_t*) table_ctx;

	if ( ! ctx->isInitialized() )
	{
		// The table context was not initialized. It means there was no trial to put any key.
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	void * key_ptr = NULL;
	void * packed_value_ptr = NULL;

	rc = ctx->tree().get(order, &key_ptr, &packed_value_ptr);
	if (rc) return rc;

	tx_debug_assert(packed_value_ptr);
	unpack_value_object(packed_value_ptr, &value->length, &value->data);

	key->length = ctx->key_length();
	key->data = key_ptr;

	return NLDB_OK;
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

	void * packed_value_ptr;
	rc = ctx->tree().del(key.data, &packed_value_ptr);
	if (rc) return rc;

	if ( packed_value_ptr )
	{
		// return the value ptr to the value pool if the key was found.
		ctx->value_pool().free(packed_value_ptr);
	}
	else
	{
		return NLDB_ERROR_KEY_NOT_FOUND;
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_array_tree_t::table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat)
{
	table_context_t * ctx = (table_context_t*)table_ctx ;

	if ( ctx->isInitialized() )
	{
		nldb_order_t key_count;
		nldb_rc_t rc = ctx->tree().get_key_count(&key_count);
		if (rc) return rc;

		table_stat->key_count = key_count;
	}
	else
	{
		// No key was inserted yet.
		table_stat->key_count = 0;
	}
	return NLDB_OK;
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

	table_context_t::tree_t::iterator_t iter_;
	table_context_t * table_ctx_;

	nldb_cursor_direction_t dir_;

	nldb_order_t key_order_;

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



nldb_rc_t nldb_plugin_array_tree_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_key_t & key) {
	cursor_context_t* the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		nldb_rc_t rc = check_key_length(the_table_ctx, key.length);
		if (rc) return rc;

		switch(direction) {
		case NLDB_CURSOR_FORWARD:
		{
			rc = the_table_ctx->tree().seek_forward( key.data, & the_cursor_ctx->iter_);
			if (rc) return rc;
			break;
		}
		case NLDB_CURSOR_BACKWARD:
		{
			rc = the_table_ctx->tree().seek_backward( key.data, & the_cursor_ctx->iter_);
			if (rc) return rc;
			break;
		}
		default:
			tx_assert(0);
		}

		// Get the key order and keep it in key_order field.
		nldb_order_t key_order;
		void * value;

		rc = the_table_ctx->tree().get(key.data, & value, & key_order);
		if (rc) return rc;

		the_cursor_ctx->dir_ = direction;
		the_cursor_ctx->key_order_ = key_order;

	}
	else
	{
		// TODO : Do we need to search again from the key if cursor_seek is called twice?
		// No nothing, just return NLDB_OK.
		// cursor_move_forward will return NLDB_ERROR_END_OF_ITERATION.
	}

	return NLDB_OK;
}


// errors : NLDB_ERROR_CURSOR_NOT_OPEN
nldb_rc_t nldb_plugin_array_tree_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_order_t & order) {
	cursor_context_t* the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		switch(direction) {
		case NLDB_CURSOR_FORWARD:
		{
			nldb_rc_t rc = the_table_ctx->tree().seek_forward( order, & the_cursor_ctx->iter_);
			if (rc) return rc;
			break;
		}
		case NLDB_CURSOR_BACKWARD:
		{
			nldb_rc_t rc = the_table_ctx->tree().seek_backward( order, & the_cursor_ctx->iter_);
			if (rc) return rc;
			break;
		}
		default:
			tx_assert(0);
		}
		the_cursor_ctx->dir_ = direction;
		the_cursor_ctx->key_order_ = order;
	}
	else
	{
		// TODO : Do we need to search again from the key if cursor_seek is called twice?
		// No nothing, just return NLDB_OK.
		// cursor_move_forward will return NLDB_ERROR_END_OF_ITERATION.
	}

	return NLDB_OK;
}


nldb_rc_t nldb_plugin_array_tree_t::cursor_fetch (nldb_cursor_context_t cursor_ctx, nldb_key_t * o_key, nldb_value_t * o_value, nldb_order_t * o_order)
{
	cursor_context_t * the_cursor_ctx = (cursor_context_t*) cursor_ctx;

	// TODO : check if cursor_seek_forward was invoked.

	table_context_t * the_table_ctx = the_cursor_ctx->table_ctx_;

	if ( the_table_ctx->isInitialized() )
	{
		bool end_of_iteration = false;
		void * key_data = NULL;
		void * packed_value_data = NULL;
		nldb_order_t order;

		switch(the_cursor_ctx->dir_) {
		case NLDB_CURSOR_FORWARD :
		{
			nldb_rc_t rc = the_table_ctx->tree().move_forward( the_cursor_ctx->iter_, & key_data, & packed_value_data, & end_of_iteration);
			if (rc) return rc;

			order = the_cursor_ctx->key_order_++;

			break;
		}
		case NLDB_CURSOR_BACKWARD :
		{
			nldb_rc_t rc = the_table_ctx->tree().move_backward( the_cursor_ctx->iter_, & key_data, & packed_value_data, & end_of_iteration);
			if (rc) return rc;

			order = the_cursor_ctx->key_order_--;

			break;
		}
		default:
			tx_assert(0);
		}

		if (end_of_iteration)
			return NLDB_ERROR_END_OF_ITERATION;

		o_key->length = the_table_ctx->key_length();
		o_key->data = key_data;

		if (o_value) {
			unpack_value_object( packed_value_data, &o_value->length, &o_value->data );
		}

		if (o_order) {
			*o_order = order;
		}
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
