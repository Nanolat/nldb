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

 
#include <string.h>

#include "nldb_test.h"
#include "nldb_data.h"

const long exepceted_value1 = 1004;
const long exepceted_value2 = 2004;

DECLARE_KEY_VALUE(rec1, 100, 100, exepceted_value1, -1);
DECLARE_KEY_VALUE(rec2, 200, 200, exepceted_value2, -1);


TEST_F(NLDBTest, transaction_commit) {

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec1) ) == 0 );

	// Check value before commit
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/ ) == 0 );
		ASSERT_TRUE( IS_VALUE_EQUAL(rec1, v));
	}

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

	// Check value after commit
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == 0 );
		ASSERT_TRUE( IS_VALUE_EQUAL(rec1, v));
	}
}

// TODO : Fix it. Transaction abort clears transaction log buffer resulting in resetting transaction ID.
TEST_F(NLDBTest, transaction_abort) {

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec1) ) == 0 );

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	// Put the value with rec2 temporarily.
	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec2) ) == 0 );

	// Check before abort
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == 0 );
		// The value should be rec2
		ASSERT_TRUE( IS_VALUE_EQUAL(rec2, v));
	}
	ASSERT_TRUE( nldb_tx_abort(tx) == 0 );

	// Check value after abort
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/ ) == 0 );
		// The value should be rec1 after aborting the transaction.
		ASSERT_TRUE( IS_VALUE_EQUAL(rec1, v));
	}

}


TEST_F(NLDBTest, get_before_put) {
	nldb_value_t v;

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );
}


TEST_F(NLDBTest, del) {
	nldb_value_t v;

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec1) ) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == 0 );
	ASSERT_TRUE( IS_VALUE_EQUAL(rec1, v));

	// Check value before commit
	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == 0 );

	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v, NULL /*nldb_order_t*/) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );
}

