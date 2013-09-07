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

class DbFunctionTest : public testing::Test {
  protected:
    virtual void SetUp() {
    	ASSERT_TRUE( nldb_init() == 0 );
    }

    virtual void TearDown() {
    	ASSERT_TRUE( nldb_destroy() == 0 );
    }
};

static const int DB_ID = 1000;
/***************************************************************************************
 * Tests for normal usage of nldb_db_xxx API.
 */
TEST_F(DbFunctionTest, create_open_db_should_succeed) {
	nldb_db_t db;
	ASSERT_EQ( nldb_db_create(DB_ID), NLDB_OK );
	/* Both master option and slave options are NULL. Open the database without any replication. */
	ASSERT_EQ( nldb_db_open(DB_ID, NULL, NULL, &db), NLDB_OK );
	ASSERT_EQ( nldb_db_close(db), NLDB_OK );
	ASSERT_EQ( nldb_db_drop(DB_ID), NLDB_OK );
}

/***************************************************************************************
 * Tests for nldb_db_create
 */
TEST_F(DbFunctionTest, create_drop_db_id_with_0_should_succeed) {
	ASSERT_EQ( nldb_db_create(0), NLDB_OK );
	ASSERT_EQ( nldb_db_drop(0), NLDB_OK );
}

TEST_F(DbFunctionTest, create_db_id_with_less_than_0_should_fail) {
	ASSERT_EQ( nldb_db_create(-1), NLDB_ERROR_INVALID_ARGUMENT );
}

TEST_F(DbFunctionTest, create_db_twice_should_fail) {
	ASSERT_EQ( nldb_db_create(DB_ID), NLDB_OK );
	ASSERT_EQ( nldb_db_create(DB_ID), NLDB_ERROR_DB_ALREADY_EXISTS );
	ASSERT_EQ( nldb_db_drop(DB_ID), NLDB_OK );
}

static const int MAX_DB_ID = 1024;

TEST_F(DbFunctionTest, create_with_less_than_max_db_id_should_succeed) {
	ASSERT_EQ( nldb_db_create(MAX_DB_ID-1), NLDB_OK );
	ASSERT_EQ( nldb_db_drop(MAX_DB_ID-1), NLDB_OK );
}

TEST_F(DbFunctionTest, create_with_max_db_id_should_succeed) {
	ASSERT_EQ( nldb_db_create(MAX_DB_ID), NLDB_OK );
	ASSERT_EQ( nldb_db_drop(MAX_DB_ID), NLDB_OK );
}

TEST_F(DbFunctionTest, create_with_greater_than_max_db_id_should_fail) {
	ASSERT_EQ( nldb_db_create(MAX_DB_ID+1), NLDB_ERROR_INVALID_ARGUMENT );
}

/***************************************************************************************
 * Tests for nldb_db_drop
 */
TEST_F(DbFunctionTest, drop_db_id_with_less_than_0_should_fail) {
	ASSERT_EQ( nldb_db_drop(-1), NLDB_ERROR_INVALID_ARGUMENT );
}

TEST_F(DbFunctionTest, drop_non_existing_db_should_fail) {
	ASSERT_EQ( nldb_db_drop(DB_ID), NLDB_ERROR_DB_NOT_FOUND );
}

TEST_F(DbFunctionTest, drop_with_greater_than_max_db_id_should_fail) {
	ASSERT_EQ( nldb_db_drop(MAX_DB_ID+1), NLDB_ERROR_INVALID_ARGUMENT );
}

/***************************************************************************************
 * Tests for nldb_db_open
 */
TEST_F(DbFunctionTest, open_db_id_with_less_than_0_should_fail) {
	nldb_db_t db;
	ASSERT_EQ( nldb_db_open(-1, NULL, NULL, &db), NLDB_ERROR_INVALID_ARGUMENT );
}

TEST_F(DbFunctionTest, open_db_id_with_0_should_succeed) {
	nldb_db_t db;
	ASSERT_EQ( nldb_db_create(0), NLDB_OK );
	ASSERT_EQ( nldb_db_open(0, NULL, NULL, &db), NLDB_OK );
	ASSERT_EQ( nldb_db_close(db), NLDB_OK );
	ASSERT_EQ( nldb_db_drop(0), NLDB_OK );
}

TEST_F(DbFunctionTest, open_db_object_with_NULL_should_fail) {
	ASSERT_EQ( nldb_db_open(1, NULL, NULL, NULL), NLDB_ERROR_INVALID_ARGUMENT );
}

TEST_F(DbFunctionTest, open_non_existing_db_should_fail) {
	nldb_db_t db;
	ASSERT_EQ( nldb_db_open(DB_ID, NULL, NULL, &db), NLDB_ERROR_DB_NOT_FOUND );
}


/***************************************************************************************
 * Tests for nldb_db_close
 */
TEST_F(DbFunctionTest, close_db_with_NULL_pointer_should_fail) {
	nldb_db_t db = NULL;
	ASSERT_EQ( nldb_db_close(db), NLDB_ERROR_INVALID_ARGUMENT );
}
