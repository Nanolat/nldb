/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
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
 */

#include <string.h>

#include "nldb_test.h"

const long exepceted_value = 1004;
DECLARE_KEY_VALUE(rec1, 100, 100, exepceted_value);


TEST_F(NLDBTest, transaction_commit) {

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec1) ) == 0 );

	// Check value before commit
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == 0 );
		ASSERT_TRUE( IS_VALUE_EQUAL(v, exepceted_value));
	}

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

	// Check value after commit
	{
		nldb_value_t v;
		ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == 0 );
		ASSERT_TRUE( IS_VALUE_EQUAL(v, exepceted_value));
	}
}

// TODO : Implement it.
/*
TEST_F(NLDBTest, transaction_abort) {
	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );


	ASSERT_TRUE( nldb_tx_abort(tx) == 0 );
}
*/

TEST_F(NLDBTest, get_before_put) {
	nldb_value_t v;

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );
}


TEST_F(NLDBTest, del) {
	nldb_value_t v;

	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(rec1), VALUE(rec1) ) == 0 );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == 0 );
	ASSERT_TRUE( IS_VALUE_EQUAL(v, exepceted_value));

	// Check value before commit
	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == 0 );

	ASSERT_TRUE( nldb_table_del( tx, vol_table, KEY(rec1)) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_table_get( tx, vol_table, KEY(rec1), & v) == NLDB_ERROR_KEY_NOT_FOUND );

	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );
}

