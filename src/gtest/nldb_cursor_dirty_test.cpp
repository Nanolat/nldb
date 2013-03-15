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
