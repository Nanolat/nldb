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
 
#ifndef NLDB_TEST_H_
#define NLDB_TEST_H_

#include <gtest/gtest.h>

// for htonl
#include <arpa/inet.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include <stdint.h>
using namespace std;

const int TEST_DB_ID = 1;
const int VOLATILE_TABLE_ID = 1;
const int PERSISTENT_TABLE_ID = 2;

class NLDBTest : public testing::Test {
  protected:
    virtual void SetUp() {
    	ASSERT_TRUE( nldb_init() == 0 );

    	(void)nldb_db_drop( TEST_DB_ID );

    	ASSERT_TRUE( nldb_db_create( TEST_DB_ID ) == 0 );

    	ASSERT_TRUE( nldb_db_open( TEST_DB_ID, NULL/*master*/, NULL/*slave*/, &db ) == 0 );

    	ASSERT_TRUE( nldb_table_create(db, VOLATILE_TABLE_ID, NLDB_TABLE_VOLATILE) == 0 );

    	ASSERT_TRUE( nldb_table_create(db, PERSISTENT_TABLE_ID, NLDB_TABLE_PERSISTENT) == 0 );

    	ASSERT_TRUE( nldb_table_open(db, VOLATILE_TABLE_ID, &vol_table) == 0 );

    	ASSERT_TRUE( nldb_table_open(db, PERSISTENT_TABLE_ID, &pers_table) == 0 );

    	ASSERT_TRUE( nldb_tx_init( db, &tx ) == 0 );
    }

    virtual void TearDown() {
    	ASSERT_TRUE( nldb_tx_destroy( tx ) == 0 );

    	ASSERT_TRUE( nldb_table_close(vol_table) == 0 );

    	ASSERT_TRUE( nldb_table_close(pers_table) == 0 );

    	ASSERT_TRUE( nldb_table_drop(db, VOLATILE_TABLE_ID) == 0 );

    	ASSERT_TRUE( nldb_table_drop(db, PERSISTENT_TABLE_ID) == 0 );

    	ASSERT_TRUE( nldb_db_close( db ) == 0 );

    	ASSERT_TRUE( nldb_db_drop( TEST_DB_ID ) == 0 );

    	ASSERT_TRUE( nldb_destroy() == 0 );
    }

	nldb_db_t db;
	nldb_table_t vol_table;
	nldb_table_t pers_table;
	nldb_tx_t tx;
};

#endif /* NLDB_TEST_H_ */
