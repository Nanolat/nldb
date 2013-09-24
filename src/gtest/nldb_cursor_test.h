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
