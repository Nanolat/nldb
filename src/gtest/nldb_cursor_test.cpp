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

using namespace std;

#include "nldb_cursor_test.h"

#include "nldb_data.h"

TEST_F(NLDBCursorTest, find_existing_fetch_forward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r6) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}


TEST_F(NLDBCursorTest, find_existing_fetch_backward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r3) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_not_existing_fetch_forward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r_5_6) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_not_existing_fetch_backward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r_3_4) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_min_fetch_forward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r4, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r5, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_min_fetch_backward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_max_fetch_forward) {
	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r8) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_max_fetch_backward) {
	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r8) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r5, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r4, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

TEST_F(NLDBCursorTest, find_before_min_fetch_forward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r_bmin) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r4, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r5, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_before_min_fetch_backward) {

	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r_bmin) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_after_max_fetch_forward) {
	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_FORWARD, KEY(r_amax) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

TEST_F(NLDBCursorTest, find_after_max_fetch_backward) {
	ASSERT_TRUE( nldb_cursor_seek( cursor, NLDB_CURSOR_BACKWARD, KEY(r_amax) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r8, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r7, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r6, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r5, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r4, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r3, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r2, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );
	EXPECT_TRUE( IS_VALUE_EQUAL( r1, value ) );

	ASSERT_TRUE( nldb_cursor_fetch( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

// TODO : Add cursor tests by calling nldb_cursor_seek with a key order
