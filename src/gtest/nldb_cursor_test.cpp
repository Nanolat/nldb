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


using namespace std;

#include "nldb_cursor_test.h"


const long exepceted_value1 = 10001;
const long exepceted_value2 = 10002;
const long exepceted_value3 = 10003;
const long exepceted_value4 = 10004;
const long exepceted_value5 = 10005;
const long exepceted_value6 = 10006;
const long exepceted_value7 = 10007;
const long exepceted_value8 = 10007;

DECLARE_KEY_VALUE(r_bmin,    10,    0, -1); // for searching purpose
DECLARE_KEY_VALUE(r1,        50,    0, exepceted_value1);
DECLARE_KEY_VALUE(r2,       100, 1000, exepceted_value2);
DECLARE_KEY_VALUE(r3,       200,  200, exepceted_value3);
DECLARE_KEY_VALUE(r_3_4,    200,  250, -1); // for searching purpose
DECLARE_KEY_VALUE(r4,       200,  300, exepceted_value4);
DECLARE_KEY_VALUE(r5,      1000,  100, exepceted_value5);
DECLARE_KEY_VALUE(r_5_6,   1000,  150, -1); // for searching purpose
DECLARE_KEY_VALUE(r6,      1000,  200, exepceted_value6);
DECLARE_KEY_VALUE(r7,      2000,  100, exepceted_value7);
DECLARE_KEY_VALUE(r8,      2000,  200, exepceted_value8);
DECLARE_KEY_VALUE(r_amax, 10000,    0, -1); // for searching purpose


TEST_F(NLDBCursorTest, find_existing_move_forward) {

	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r6) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}


TEST_F(NLDBCursorTest, find_existing_move_backward) {

	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r3) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_not_existing_move_forward) {

	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r_5_6) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_not_existing_move_backward) {

	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r_3_4) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_min_move_forward) {

	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_min_move_backward) {

	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_max_move_forward) {
	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r8) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_max_move_backward) {
	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r8) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

TEST_F(NLDBCursorTest, find_before_min_move_forward) {

	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r_bmin) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );

}

TEST_F(NLDBCursorTest, find_before_min_move_backward) {

	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r_bmin) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}


TEST_F(NLDBCursorTest, find_after_max_move_forward) {
	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r_amax) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

TEST_F(NLDBCursorTest, find_after_max_move_backward) {
	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r_amax) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r8, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r7, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r6, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r5, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r4, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r3, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r2, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( r1, key ) );

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_END_OF_ITERATION );
}

