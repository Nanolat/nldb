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

