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
