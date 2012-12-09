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

#ifndef NLDB_CURSOR_TEST_H_
#define NLDB_CURSOR_TEST_H_

#include "nldb_test.h"

DEFINE_KEY_VALUE(r_bmin); // for searching purpose
DEFINE_KEY_VALUE(r1);
DEFINE_KEY_VALUE(r2);
DEFINE_KEY_VALUE(r3);
DEFINE_KEY_VALUE(r_3_4); // for searching purpose
DEFINE_KEY_VALUE(r4);
DEFINE_KEY_VALUE(r5);
DEFINE_KEY_VALUE(r_5_6); // for searching purpose
DEFINE_KEY_VALUE(r6);
DEFINE_KEY_VALUE(r7);
DEFINE_KEY_VALUE(r8);
DEFINE_KEY_VALUE(r_amax); // for searching purpose

// Run cursor tests on persistent table, which uses LevelDB.
// Volatile table uses TokyoCabinet, which does not support cursors.
class NLDBCursorTest : public NLDBTest {
  protected:
    virtual void SetUp() {
    	NLDBTest::SetUp();

    	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

    	// put values with random order.
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r3), VALUE(r3) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r7), VALUE(r7) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r2), VALUE(r2) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r1), VALUE(r1) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r4), VALUE(r4) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r5), VALUE(r5) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r8), VALUE(r8) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, pers_table, KEY(r6), VALUE(r6) ) == 0 );


    	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

		ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

		ASSERT_TRUE( nldb_cursor_open(tx, pers_table, &cursor) == 0 );

    }

    virtual void TearDown() {
		ASSERT_TRUE( nldb_cursor_close( cursor ) == 0 );

		ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

    	NLDBTest::TearDown();
    }

	nldb_cursor_t cursor;

};


#endif /* NLDB_CURSOR_TEST_H_ */
