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
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r3), VALUE(r3) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r7), VALUE(r7) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r2), VALUE(r2) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r1), VALUE(r1) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r4), VALUE(r4) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r5), VALUE(r5) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r8), VALUE(r8) ) == 0 );
    	ASSERT_TRUE( nldb_table_put( tx, vol_table, KEY(r6), VALUE(r6) ) == 0 );


    	ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

		ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

		ASSERT_TRUE( nldb_cursor_open(tx, vol_table, &cursor) == 0 );

    }

    virtual void TearDown() {
		ASSERT_TRUE( nldb_cursor_close( cursor ) == 0 );

		ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

    	NLDBTest::TearDown();
    }

	nldb_cursor_t cursor;

};


#endif /* NLDB_CURSOR_TEST_H_ */
