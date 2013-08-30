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

#include "nldb_order_test.h"

#include "nldb_data.h"

const long exepceted_value1 = 10001;
const long exepceted_value2 = 10002;
const long exepceted_value3 = 10003;
const long exepceted_value4 = 10004;
const long exepceted_value5 = 10005;
const long exepceted_value6 = 10006;
const long exepceted_value7 = 10007;
const long exepceted_value8 = 10008;
const long exepceted_value9 = 10009;
const long exepceted_value10 = 10010;
const long exepceted_value11 = 10011;
const long exepceted_value12 = 10012;

DECLARE_KEY_VALUE(o_r1,   50, 0,   exepceted_value1, -1);
DECLARE_KEY_VALUE(o_r2,   50, 200, exepceted_value2, -1);
DECLARE_KEY_VALUE(o_r3,  100, 0,   exepceted_value3, -1);
DECLARE_KEY_VALUE(o_r4,  100, 200, exepceted_value4, -1);
DECLARE_KEY_VALUE(o_r5,  200, 0,   exepceted_value5, -1);
DECLARE_KEY_VALUE(o_r6,  300, 0,   exepceted_value6, -1);
DECLARE_KEY_VALUE(o_r7,  300, 20,  exepceted_value7, -1);
DECLARE_KEY_VALUE(o_r8,  400, 0,   exepceted_value8, -1);
DECLARE_KEY_VALUE(o_r9,  400, 10,  exepceted_value9, -1);
DECLARE_KEY_VALUE(o_r10, 700, 0,   exepceted_value10, -1);
DECLARE_KEY_VALUE(o_r11, 800, 0,   exepceted_value11, -1);
DECLARE_KEY_VALUE(o_r12, 900, 0,   exepceted_value12, -1);

TEST_F(NLDBOrderTest, no_keys) {
	nldb_key_t key;
	nldb_value_t value;
	nldb_order_t order;
	nldb_table_stat_t stat;

	// Check key count
	EXPECT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 0 );

	// Get by order should return error.
	EXPECT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) -1, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	EXPECT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	EXPECT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// No key should exist at all
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r5), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r6), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r7), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r8), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r9), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r10), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r11), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	EXPECT_EQ( nldb_table_get( tx, vol_table, KEY(o_r12), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
}

// Get the value and order on a table searching by key
TEST_F(NLDBOrderTest, max_5_keys) {
	nldb_key_t key;
	nldb_value_t value;

	nldb_order_t order;
	nldb_table_stat_t stat;

	////////////////////////////////////////////////////////////////////////////////
	// One Key : o_r1
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r1), VALUE(o_r1) ), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value ) );
	EXPECT_EQ( order, 1);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r1, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value));

	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 1 );

	////////////////////////////////////////////////////////////////////////////////
	// Two keys : o_r1 -> o_r2
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r2), VALUE(o_r2) ), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value ) );
	EXPECT_EQ( order, 1);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 2);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r1, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));


	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 3, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 2 );

	////////////////////////////////////////////////////////////////////////////////
	// One key : o_r2
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r1)), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 1);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));

	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 1 );

	////////////////////////////////////////////////////////////////////////////////
	// Three Keys : o_r2->o_r3->o_r4
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r4), VALUE(o_r4) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r3), VALUE(o_r3) ), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 1);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 2);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value ) );
	EXPECT_EQ( order, 3);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r3, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 3, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r4, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value));


	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 4, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 3 );

	////////////////////////////////////////////////////////////////////////////////
	// Four Keys : o_r1->o_r2->o_r3->o_r4
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r1), VALUE(o_r1) ), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value ) );
	EXPECT_EQ( order, 1);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 2);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 3);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value ) );
	EXPECT_EQ( order, 4);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r1, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 3, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r3, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 4, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r4, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value));


	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 5, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 4 );

	////////////////////////////////////////////////////////////////////////////////
	// Five Keys : o_r2->o_r4->o_r5->o_r7->o_r8
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r6), VALUE(o_r6) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r5), VALUE(o_r5) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r8), VALUE(o_r8) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r7), VALUE(o_r7) ), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r1)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r3)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r6)), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 1);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value ) );
	EXPECT_EQ( order, 2);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r5), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value ) );
	EXPECT_EQ( order, 3);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r6), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r7), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value ) );
	EXPECT_EQ( order, 4);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r8), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r8, value ) );
	EXPECT_EQ( order, 5);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r4, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 3, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r5, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 4, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r7, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 5, &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r8, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r8, value));

	// Get by order - Out of range
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 6, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 5 );

	////////////////////////////////////////////////////////////////////////////////
    // Remove all keys
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r2)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r4)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r5)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r7)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r8)), 0 );

	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 0 );
}

// Get the key and value on a table searching by order
TEST_F(NLDBOrderTest, max_12_keys) {
	nldb_key_t key;

	nldb_value_t value;
	nldb_order_t order;

	nldb_table_stat_t stat;


	////////////////////////////////////////////////////////////////////////////////
	// 12 Keys : o_r1 -> o_r2 -> o_r3 -> ... -> o_r12

	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r9), VALUE(o_r9) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r5), VALUE(o_r5) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r12), VALUE(o_r12) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r3), VALUE(o_r3) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r8), VALUE(o_r8) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r2), VALUE(o_r2) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r6), VALUE(o_r6) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r4), VALUE(o_r4) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r10), VALUE(o_r10) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r11), VALUE(o_r11) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r7), VALUE(o_r7) ), 0 );
	ASSERT_EQ( nldb_table_put( tx, vol_table, KEY(o_r1), VALUE(o_r1) ), 0 );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value ) );
	EXPECT_EQ( order, 1);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value ) );
	EXPECT_EQ( order, 2);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value ) );
	EXPECT_EQ( order, 3);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value ) );
	EXPECT_EQ( order, 4);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r5), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value ) );
	EXPECT_EQ( order, 5);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r6), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r6, value ) );
	EXPECT_EQ( order, 6);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r7), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value ) );
	EXPECT_EQ( order, 7);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r8), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r8, value ) );
	EXPECT_EQ( order, 8);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r9), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value ) );
	EXPECT_EQ( order, 9);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r10), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r10, value ) );
	EXPECT_EQ( order, 10);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r11), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value ) );
	EXPECT_EQ( order, 11);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r12), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r12, value ) );
	EXPECT_EQ( order, 12);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)1 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r1, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)2 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r2, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r2, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)3 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r3, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)4 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r4, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r4, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)5 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r5, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)6 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r6, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r6, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)7 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r7, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)8 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r8, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r8, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)9 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r9, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)10 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r10, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r10, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)11 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r11, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)12 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r12, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r12, value));


	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 13, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 12 );

	////////////////////////////////////////////////////////////////////////////////
	// 6 Keys : o_r1 -> o_r3 -> o_r5 -> o_r7 -> o_r9 -> o_r11

	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r2)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r4)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r6)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r8)) , 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r10)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r12)), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value ) );
	EXPECT_EQ( order, 1);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value ) );
	EXPECT_EQ( order, 2);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r5), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value ) );
	EXPECT_EQ( order, 3);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r6), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r7), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value ) );
	EXPECT_EQ( order, 4);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r8), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r9), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value ) );
	EXPECT_EQ( order, 5);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r10), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r11), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value ) );
	EXPECT_EQ( order, 6);


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r12), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)1 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r1, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r1, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)2 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r3, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r3, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)3 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r5, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r5, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)4 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r7, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)5 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r9, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)6 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r11, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value));

	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 7, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 6 );

	////////////////////////////////////////////////////////////////////////////////
	// 3 Keys : o_r7 -> o_r9 -> o_r11

	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r1)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r3)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r5)), 0 );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r1), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r2), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r3), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r4), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r5), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );
	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r6), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r7), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value ) );
	EXPECT_EQ( order, 1);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r8), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r9), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value ) );
	EXPECT_EQ( order, 2);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r10), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r11), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value ) );
	EXPECT_EQ( order, 3);

	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r12), & value, &order), NLDB_ERROR_KEY_NOT_FOUND );

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)1 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r7, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r7, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)2 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r9, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value));

	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)3 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r11, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r11, value));

	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 4, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 3 );

	////////////////////////////////////////////////////////////////////////////////
	// 1 Key : o_r9
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r7)), 0 );
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r11)), 0 );


	ASSERT_EQ( nldb_table_get( tx, vol_table, KEY(o_r9), & value, &order), 0 );
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value ) );
	EXPECT_EQ( order, 1);

	// Get by order
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t)1 , &key, &value), 0 );
	EXPECT_TRUE( IS_KEY_EQUAL( o_r9, key));
	EXPECT_TRUE( IS_VALUE_EQUAL( o_r9, value));

	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 2, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 1 );

	////////////////////////////////////////////////////////////////////////////////
    // Remove all keys
	ASSERT_EQ( nldb_table_del( tx, vol_table, KEY(o_r9)), 0 );

	// Get by order should return error.
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 0, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );
	ASSERT_EQ( nldb_table_get( tx, vol_table, (nldb_order_t) 1, &key, &value), NLDB_ERROR_ORDER_OUT_OF_RANGE );

	// Check key count
	ASSERT_EQ( nldb_table_stat(tx, vol_table, &stat), 0 );
	EXPECT_EQ( stat.key_count, 0 );
}



