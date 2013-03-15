/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
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
 */

#ifndef NLDB_TEST_H_
#define NLDB_TEST_H_

#include <gtest/gtest.h>

// for htonl
#include <arpa/inet.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include <stdint.h>
using namespace std;

const int TEST_DB_ID = 1;
const int VOLATILE_TABLE_ID = 1;
const int PERSISTENT_TABLE_ID = 2;

#define KEY_SIZE = 8;
#define VALUE_SIZE = 256;

typedef struct tbl_key {
	uint32_t key1;
	uint32_t key2;
} tbl_key ;
typedef struct tbl_value{
	long value;
} tbl1_value ;


class NLDBTest : public testing::Test {
  protected:
    virtual void SetUp() {
    	ASSERT_TRUE( nldb_init() == 0 );

    	ASSERT_TRUE( nldb_db_create( TEST_DB_ID ) == 0 );

    	ASSERT_TRUE( nldb_db_open( TEST_DB_ID, NULL/*master*/, NULL/*slave*/, &db ) == 0 );

    	ASSERT_TRUE( nldb_table_create(db, VOLATILE_TABLE_ID, NLDB_TABLE_VOLATILE) == 0 );

    	ASSERT_TRUE( nldb_table_create(db, PERSISTENT_TABLE_ID, NLDB_TABLE_PERSISTENT) == 0 );

    	ASSERT_TRUE( nldb_table_open(db, VOLATILE_TABLE_ID, &vol_table) == 0 );

    	ASSERT_TRUE( nldb_table_open(db, PERSISTENT_TABLE_ID, &pers_table) == 0 );

    	ASSERT_TRUE( nldb_tx_init( db, &tx ) == 0 );
    }

    virtual void TearDown() {
    	ASSERT_TRUE( nldb_tx_destroy( tx ) == 0 );

    	ASSERT_TRUE( nldb_table_close(vol_table) == 0 );

    	ASSERT_TRUE( nldb_table_close(pers_table) == 0 );

    	ASSERT_TRUE( nldb_table_drop(db, VOLATILE_TABLE_ID) == 0 );

    	ASSERT_TRUE( nldb_table_drop(db, PERSISTENT_TABLE_ID) == 0 );

    	ASSERT_TRUE( nldb_db_close( db ) == 0 );

    	ASSERT_TRUE( nldb_db_drop( TEST_DB_ID ) == 0 );

    	ASSERT_TRUE( nldb_destroy() == 0 );
    }

	nldb_db_t db;
	nldb_table_t vol_table;
	nldb_table_t pers_table;
	nldb_tx_t tx;
};

// We can't call htonl within the list of initializing values of global variables.
// However, macro function can be used, so we define htonl_macro here.
#define switch_endian32(v) (unsigned int)( ((((unsigned int)v)&0x000000FFU) << 24) | \
                                           ((((unsigned int)v)&0x0000FF00U) << 8)  | \
                                           ((((unsigned int)v)&0x00FF0000U) >> 8)    | \
                                           ((((unsigned int)v)&0xFF000000U) >> 24) )


#define DEFINE_KEY_VALUE(var) \
extern tbl_key      var##_key; \
extern tbl_value    var##_value; \
extern nldb_key_t   var##_nldb_key; \
extern nldb_value_t var##_nldb_value;

// About using htonl : Keys are stored in big endian encoding in NLDB.
// This is to quickly compare keys by calling memcmp on keys whenever NLDB searches a key.
#define DECLARE_KEY_VALUE(var, k1, k2, v) \
tbl_key      var##_key = {switch_endian32(k1), switch_endian32(k2)}; \
tbl_value    var##_value = {v}; \
nldb_key_t   var##_nldb_key = { & var##_key, sizeof(var##_key) }; \
nldb_value_t var##_nldb_value = { & var##_value, sizeof(var##_value) };

#define KEY(var)    var##_nldb_key
#define VALUE(var)  var##_nldb_value

#define IS_VALUE_EQUAL(nldbValue,expectedValue) (memcmp(nldbValue.data, &expectedValue, v.length) == 0)
#define IS_KEY_EQUAL(var, expectedKey) ( (var##_nldb_key.length == expectedKey.length) && (memcmp(var##_nldb_key.data, expectedKey.data, expectedKey.length) == 0) )


#endif /* NLDB_TEST_H_ */
