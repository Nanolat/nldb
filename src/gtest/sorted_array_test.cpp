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

using namespace std;

#include <gtest/gtest.h>

// for htonl
#include <arpa/inet.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include "tree_test_common.h"

class SortedArrayTest : public testing::Test  {
  protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(SortedArrayTest, compare_keys) {
}

TEST_F(SortedArrayTest, next_key) {
}

TEST_F(SortedArrayTest, prev_key) {
}

TEST_F(SortedArrayTest, search_forward) {
}

TEST_F(SortedArrayTest, search_backward) {
}

TEST_F(SortedArrayTest, set_data) {
}

TEST_F(SortedArrayTest, get_data) {
}

TEST_F(SortedArrayTest, get_key) {
}

TEST_F(SortedArrayTest, move_keys_right_by_the_key_length_from) {
}

TEST_F(SortedArrayTest, move_keys_left_by_the_key_length_from) {
}

TEST_F(SortedArrayTest, set_key_data_at) {
}

TEST_F(SortedArrayTest, move_data_right_by_one_from) {
}

TEST_F(SortedArrayTest, move_data_left_by_one_from) {
}

TEST_F(SortedArrayTest, insert_key_data_at) {
}

TEST_F(SortedArrayTest, remove_key_data_at) {
}

TEST_F(SortedArrayTest, init_destroy) {
}

TEST_F(SortedArrayTest, init_copy) {
}

TEST_F(SortedArrayTest, put) {
}

TEST_F(SortedArrayTest, get) {
}

TEST_F(SortedArrayTest, find_first_le_key) {
}

TEST_F(SortedArrayTest, del) {
}

TEST_F(SortedArrayTest, remove_max_key) {
}

TEST_F(SortedArrayTest, iter_forward_iter_next) {
}

TEST_F(SortedArrayTest, iter_backward_iter_prev) {
}

TEST_F(SortedArrayTest, merge_with) {
}

TEST_F(SortedArrayTest, split) {
}

TEST_F(SortedArrayTest, key_count) {
}

TEST_F(SortedArrayTest, key_length) {
}

TEST_F(SortedArrayTest, used_key_space) {
}

TEST_F(SortedArrayTest, is_empty) {
}

TEST_F(SortedArrayTest, is_full) {
}

TEST_F(SortedArrayTest, min_key) {
}
