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

#ifndef NLDB_CURSOR_TEST_H_
#define NLDB_CURSOR_TEST_H_

#include "nldb_test.h"
#include "nldb_data.h"

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
