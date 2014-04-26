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

// Run order tests; Put/Del keys and check if the order(n-th key) returned by nldb is correct.
class NLDBOrderTest : public NLDBTest {
  protected:
    virtual void SetUp() {
    	NLDBTest::SetUp();

    	ASSERT_TRUE( nldb_tx_begin(tx) == 0 );

    }

    virtual void TearDown() {

		ASSERT_TRUE( nldb_tx_commit(tx) == 0 );

    	NLDBTest::TearDown();
    }

};


#endif /* NLDB_CURSOR_TEST_H_ */
