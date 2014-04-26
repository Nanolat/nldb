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

class NLDBTest : public testing::Test {
  protected:
    virtual void SetUp() {
    	ASSERT_TRUE( nldb_init() == 0 );

    	(void)nldb_db_drop( TEST_DB_ID );

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

#endif /* NLDB_TEST_H_ */
