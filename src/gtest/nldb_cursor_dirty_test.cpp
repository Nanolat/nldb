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

using namespace std;

#include "nldb_cursor_test.h"


class NLDBCursorDirtyTest : public NLDBCursorTest {
  protected:
    virtual void SetUp() {
    	NLDBCursorTest::SetUp();
    }

    virtual void TearDown() {
		NLDBCursorTest::TearDown();
    }
};
/*
TEST_F(NLDBCursorDirtyTest, seek_forward_move_backward) {

	ASSERT_TRUE( nldb_cursor_seek_forward( cursor, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_backward( cursor,  &key,  &value ) == NLDB_ERROR_INVALID_ITERATION );
}

TEST_F(NLDBCursorDirtyTest, seek_backward_move_forward) {

	ASSERT_TRUE( nldb_cursor_seek_backward( cursor, KEY(r1) ) == 0);

	nldb_key_t key;
	nldb_value_t value;

	ASSERT_TRUE( nldb_cursor_move_forward( cursor,  &key,  &value ) == NLDB_ERROR_INVALID_ITERATION );
}
*/
