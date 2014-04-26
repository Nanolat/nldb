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
