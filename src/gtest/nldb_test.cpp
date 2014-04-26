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

