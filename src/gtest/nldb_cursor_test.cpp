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

