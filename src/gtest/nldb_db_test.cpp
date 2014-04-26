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
