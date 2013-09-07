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


#include <txbase/tx_assert.h>

#include "nldb_plugin_leveldb.h"

#include <leveldb/db.h>
#include <sstream>

typedef struct levedb_table_desc_t {
	nldb_table_id_t db_id;
	nldb_table_id_t table_id;
} levedb_table_desc_t;

static std::string leveldb_name(const nldb_table_id_t db_id, const nldb_table_id_t table_id)
{
	std::ostringstream os;
	os <<  "nldb-" << db_id << "-persistent-table-" << table_id;
	return os.str();
}

static std::string leveldb_name(nldb_plugin_table_desc_t & table_desc)
{
	levedb_table_desc_t * p_leveldb_table_desc = (levedb_table_desc_t*) table_desc.table_desc;

	return leveldb_name( p_leveldb_table_desc->db_id, p_leveldb_table_desc->table_id );
}

// The descriptor for the meta table that holds the list of tables 
nldb_plugin_table_desc_t nldb_meta_table_desc(const nldb_db_id_t db_id)
{
	nldb_plugin_table_desc_t meta_table_desc;
	levedb_table_desc_t * p_leveldb_table_desc = (levedb_table_desc_t*) meta_table_desc.table_desc;

	p_leveldb_table_desc->db_id = db_id;
	p_leveldb_table_desc->table_id = 0;

	meta_table_desc.length = sizeof(levedb_table_desc_t);

	return meta_table_desc;
}

static void fatal_error(const std::string & error_message) {
	printf("leveldb plugin fatal error : %s\n", error_message.c_str());
}

nldb_rc_t nldb_table_exists(nldb_plugin_table_desc_t & table_desc, bool * exists)
{
	leveldb::DB* db = NULL;
	leveldb::Options options;
	options.create_if_missing = false;

	leveldb::Status status = leveldb::DB::Open(options, leveldb_name( table_desc ), &db);

	// BUGBUG : return error code.
	if (status.ok()) {
		*exists = true;
		tx_assert(db);
		delete db;
	} else {
		*exists = false;
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::table_create(const nldb_table_id_t db_id, const nldb_table_id_t table_id, nldb_plugin_table_desc_t * table_desc)
{
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;

	std::string dbname = leveldb_name(db_id, table_id);

	leveldb::Status status = leveldb::DB::Open(options, dbname, &db);

	// BUGBUG : return error code.
	if(!status.ok()) {
		fatal_error(status.ToString());
		tx_assert(0);
	}

	if ( ! db ) {
		return NLDB_ERROR;
	}

	// Close the DB.
	delete db;

	// Set the table descriptor
	levedb_table_desc_t * p_leveldb_table_desc = (levedb_table_desc_t*) table_desc->table_desc;

	p_leveldb_table_desc->db_id = db_id;
	p_leveldb_table_desc->table_id = table_id;

	table_desc->length = sizeof(levedb_table_desc_t);

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::table_drop(nldb_plugin_table_desc_t & table_desc)
{
	leveldb::Options options;

	leveldb::Status status = leveldb::DestroyDB( leveldb_name( table_desc ), options);

	// BUGBUG : return error code.
	if(!status.ok()) {
		fatal_error(status.ToString());
		tx_assert(0);
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx)
{
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status status = leveldb::DB::Open(options, leveldb_name( table_desc ), &db);

	// BUGBUG : return error code.
	if(!status.ok()) {
		fatal_error(status.ToString());
		tx_assert(0);
	}

	if ( ! db ) {
		return NLDB_ERROR;
	}

	*table_ctx = db;

	return NLDB_OK;
}


nldb_rc_t nldb_plugin_leveldb_t::table_close(nldb_table_context_t table_ctx)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	// deleting db results in closing the leveldb.
	delete db;

	return NLDB_OK;
}

static inline const leveldb::Slice get_slice(const nldb_key_t & key) {
	return leveldb::Slice((const char*)key.data, key.length);
}

static inline const leveldb::Slice get_slice(const nldb_value_t & value) {
	return leveldb::Slice((const char*)value.data, value.length);
}

static inline nldb_rc_t copy_key_from_slice(nldb_key_t * key, const leveldb::Slice & slice) {
	void * dataPtr = malloc( slice.size() );
	if (dataPtr == NULL )
		return NLDB_ERROR_NO_MEM;

	memcpy(dataPtr, slice.data(), slice.size());

	key->length = slice.size();
	key->data = dataPtr;

	return NLDB_OK;
}

static inline nldb_rc_t copy_value_from_slice(nldb_value_t * value, const leveldb::Slice & slice) {
	void * dataPtr = malloc( slice.size() );
	if (dataPtr == NULL )
		return NLDB_ERROR_NO_MEM;

	memcpy(dataPtr, slice.data(), slice.size());

	value->length = slice.size();
	value->data = dataPtr;

	return NLDB_OK;
}


/*
static inline const nldb_value_t get_value(const std::string & str) {
	nldb_value_t value = {(void*)str.data(), str.size()};
	return value;
}
*/

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
nldb_rc_t nldb_plugin_leveldb_t::table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	leveldb::Status status = db->Put(leveldb::WriteOptions(), get_slice(key), get_slice(value));

	return NLDB_OK;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_leveldb_t::table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order )
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	if (order != NULL)
		return NLDB_ERROR_UNSUPPORTED_FEATURE;

	std::string gotValue;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), get_slice(key), &gotValue);

	if (status.IsNotFound()) 
		return NLDB_ERROR_KEY_NOT_FOUND;

	// BUGBUG return error code.
	if(!status.ok()) {
		fatal_error(status.ToString());
		tx_assert(0);
	}

	// BUGBUG : improve performance by implementing malloc inside of db->Get and by passing value ptr to it.
	nldb_rc_t rc =copy_value_from_slice(value, gotValue );
	if (rc) return rc;

	return NLDB_OK;
}

// errors : NLDB_ERROR_ORDER_OUT_OF_RANGE
nldb_rc_t nldb_plugin_leveldb_t::table_get(nldb_table_context_t table_ctx, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_leveldb_t::table_del(nldb_table_context_t table_ctx, const nldb_key_t & key)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	// BUGBUG : Optimize by modifying db->Delete to set IsNotFound when the key is not found.
	// Because db->Delete does not set IsNotFound when the key is not found, we need to check if the key exists.
	std::string gotValue;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), get_slice(key), &gotValue);

	if (status.IsNotFound()) 
		return NLDB_ERROR_KEY_NOT_FOUND;

	// The key exists. delete it.
	status = db->Delete(leveldb::WriteOptions(), get_slice(key));

	// BUGBUG return error code.
	if(!status.ok()) {
		fatal_error(status.ToString());
		tx_assert(0);
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

nldb_rc_t nldb_plugin_leveldb_t::value_free(nldb_value_t & value) {
	free(value.data);
	value.length = 0;

	return NLDB_OK;
}


nldb_rc_t nldb_plugin_leveldb_t::key_free(nldb_key_t & key) {
	free(key.data);
	key.length = 0;

	return NLDB_OK;
}


class leveldb_cursor_context_t
{

public :
	leveldb::Iterator* iter_;

	nldb_cursor_direction_t dir_;

	leveldb_cursor_context_t(leveldb::DB* db)
	{
		iter_ = db->NewIterator(leveldb::ReadOptions());

	}
};

nldb_rc_t nldb_plugin_leveldb_t::cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	leveldb_cursor_context_t * levedb_cursor = new leveldb_cursor_context_t(db);
	tx_debug_assert(levedb_cursor);

	*cursor_ctx = levedb_cursor;

	return NLDB_OK;
}


nldb_rc_t nldb_plugin_leveldb_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_key_t & key)
{
	leveldb_cursor_context_t* leveldb_cursor = (leveldb_cursor_context_t*) cursor_ctx;
	leveldb_cursor->dir_ = direction;

	leveldb::Iterator* it = leveldb_cursor->iter_;


	it->Seek( get_slice(key) );

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

	return NLDB_OK;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN
nldb_rc_t nldb_plugin_leveldb_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_order_t & order)
{
	// Searching by key order is not suppored by leveldb.
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}


// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
nldb_rc_t nldb_plugin_leveldb_t::cursor_fetch(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value, nldb_order_t * order)
{
	leveldb_cursor_context_t* leveldb_cursor = (leveldb_cursor_context_t*) cursor_ctx;

	leveldb::Iterator* it = leveldb_cursor->iter_;

	if (order != NULL) {
		// Fetching key order is not supported by LevelDB. The order parameter should be passed as NULL always.
		return NLDB_ERROR_UNSUPPORTED_FEATURE;
	}

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

	if ( ! it->Valid())
	{
		return NLDB_ERROR_END_OF_ITERATION;
	}

	nldb_rc_t rc = copy_key_from_slice(key, it->key());
	if (rc) return rc;

	rc = copy_value_from_slice(value, it->value());
	if (rc) return rc;

	switch(leveldb_cursor->dir_) {
	case NLDB_CURSOR_FORWARD :
	    it->Next();
		break;
	case NLDB_CURSOR_BACKWARD :
	    it->Prev();
		break;
	default :
		tx_assert(0);
	}

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

   	return NLDB_OK;
}


nldb_rc_t nldb_plugin_leveldb_t::cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx)
{
	leveldb_cursor_context_t* leveldb_cursor = (leveldb_cursor_context_t*) cursor_ctx;

	leveldb::Iterator* it = leveldb_cursor->iter_;

	delete it;

	delete leveldb_cursor;

	return NLDB_OK;
}
