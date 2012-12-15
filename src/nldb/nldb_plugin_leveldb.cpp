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

#include <txbase/tx_assert.h>

#include "nldb_plugin_leveldb.h"

#include <leveldb/db.h>
#include <sstream>

typedef struct levedb_table_desc_t {
	nldb_table_id_t db_id;
	nldb_table_id_t table_id;
} levedb_table_desc_t;

// The descriptor for the meta table that holds the list of tables 
nldb_plugin_table_desc_t nldb_meta_table_desc(const nldb_db_id_t & db_id)
{
	nldb_plugin_table_desc_t meta_table_desc;
	levedb_table_desc_t * p_leveldb_table_desc = (levedb_table_desc_t*) meta_table_desc.table_desc;

	p_leveldb_table_desc->db_id = db_id;
	p_leveldb_table_desc->table_id = 0;

	meta_table_desc.length = sizeof(levedb_table_desc_t);

	return meta_table_desc;
}

static std::string leveldb_name(const nldb_table_id_t & db_id, const nldb_table_id_t & table_id)
{
	std::ostringstream os;
	os << db_id << "-" << table_id;
	return os.str();
}

static std::string leveldb_name(nldb_plugin_table_desc_t & table_desc)
{
	levedb_table_desc_t * p_leveldb_table_desc = (levedb_table_desc_t*) table_desc.table_desc;

	return leveldb_name( p_leveldb_table_desc->db_id, p_leveldb_table_desc->table_id );
}

nldb_rc_t nldb_plugin_leveldb_t::table_create(const nldb_table_id_t & db_id, const nldb_table_id_t & table_id, nldb_plugin_table_desc_t * table_desc)
{
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;

	std::string dbname = leveldb_name(db_id, table_id);

	leveldb::Status status = leveldb::DB::Open(options, dbname, &db);

	// BUGBUG : return error code.
	tx_assert(status.ok());

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
	tx_assert(status.ok());

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx)
{
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status status = leveldb::DB::Open(options, leveldb_name( table_desc ), &db);

	// BUGBUG : return error code.
	tx_assert(status.ok());

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
nldb_rc_t nldb_plugin_leveldb_t::table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	std::string gotValue;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), get_slice(key), &gotValue);

	if (status.IsNotFound()) 
		return NLDB_ERROR_KEY_NOT_FOUND;

	// BUGBUG return error code.
	tx_assert(status.ok());

	// BUGBUG : improve performance by implementing malloc inside of db->Get and by passing value ptr to it.
	nldb_rc_t rc =copy_value_from_slice(value, gotValue );
	if (rc) return rc;

	return NLDB_OK;
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
	tx_assert(status.ok());

	return NLDB_OK;
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

nldb_rc_t nldb_plugin_leveldb_t::cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx)
{
	leveldb::DB* db = (leveldb::DB*) table_ctx;

	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

	*cursor_ctx = it;

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::cursor_seek_forward (nldb_cursor_context_t cursor_ctx, const nldb_key_t & key)
{
	leveldb::Iterator* it = (leveldb::Iterator*) cursor_ctx;

	it->Seek( get_slice(key) );

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::cursor_seek_backward(nldb_cursor_context_t cursor_ctx, const nldb_key_t & key)
{
	return cursor_seek_forward(cursor_ctx, key);
}

nldb_rc_t nldb_plugin_leveldb_t::cursor_move_forward (nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value)
{
	leveldb::Iterator* it = (leveldb::Iterator*) cursor_ctx;

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

    it->Next();

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

   	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::cursor_move_backward(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value)
{
	leveldb::Iterator* it = (leveldb::Iterator*) cursor_ctx;

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

	if ( ! it->Valid() )
	{
		return NLDB_ERROR_END_OF_ITERATION;
	}

	nldb_rc_t rc = copy_key_from_slice(key, it->key());
	if (rc) return rc;

	rc = copy_value_from_slice(value, it->value());
	if (rc) return rc;

    it->Prev();

	if ( ! it->status().ok() )
	{
		return NLDB_ERROR;
	}

   	return NLDB_OK;
}

nldb_rc_t nldb_plugin_leveldb_t::cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx)
{

	leveldb::Iterator* it = (leveldb::Iterator*) cursor_ctx;

	delete it;

	return NLDB_OK;
}
