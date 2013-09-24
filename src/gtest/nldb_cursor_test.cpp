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
