/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and others.
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
 *     Saryong Kang
 */

using namespace std;

#include <gtest/gtest.h>

// for htonl
#include <arpa/inet.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include "tree_test_common.h"

class TreeInternalNodeTest : public testing::Test  {
  protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};


TEST_F(TreeInternalNodeTest, is_empty) {
	tree_internal_node_t internal(TREE_KEY_LEN);
	EXPECT_TRUE( internal.is_empty() );

	ASSERT_TRUE( internal.put(KEY_01, NODE_01) == NLDB_OK );

	EXPECT_TRUE( ! internal.is_empty() );

	tree_node_t * node;
	ASSERT_TRUE( internal.del(KEY_01, &node) == NLDB_OK );

	EXPECT_TRUE( internal.is_empty() );
}

TEST_F(TreeInternalNodeTest, is_full) {
	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	EXPECT_TRUE( ! internal.is_full() );
	ASSERT_TRUE( internal.put(KEY_01, NODE_01) == NLDB_OK );

	EXPECT_TRUE( ! internal.is_full() );
	ASSERT_TRUE( internal.put(KEY_02, NODE_02) == NLDB_OK );

	EXPECT_TRUE( ! internal.is_full() );
	ASSERT_TRUE( internal.put(KEY_03, NODE_03) == NLDB_OK );

	EXPECT_TRUE( internal.is_full() );


	tree_node_t * node = NULL;
	ASSERT_TRUE( internal.del(KEY_01, &node) == NLDB_OK );
	EXPECT_TRUE( ! internal.is_full() );

	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( ! internal.is_full() );

	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( ! internal.is_full() );
}

TEST_F(TreeInternalNodeTest, min_key) {
	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	EXPECT_TRUE( internal.min_key() == NULL );

	ASSERT_TRUE( internal.put(KEY_02, NODE_02) == NLDB_OK );
	EXPECT_TRUE( KEY_02_MATCHES( internal.min_key() )  );

	ASSERT_TRUE( internal.put(KEY_01, NODE_01) == NLDB_OK );
	EXPECT_TRUE( KEY_01_MATCHES( internal.min_key() )  );

	ASSERT_TRUE( internal.put(KEY_03, NODE_03) == NLDB_OK );
	EXPECT_TRUE( KEY_01_MATCHES( internal.min_key() )  );

	tree_node_t * node = NULL;

	ASSERT_TRUE( internal.del(KEY_01, &node) == NLDB_OK );
	EXPECT_TRUE( KEY_02_MATCHES( internal.min_key() )  );

	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( KEY_03_MATCHES( internal.min_key() )  );

	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( internal.min_key() == NULL );
}


TEST_F(TreeInternalNodeTest, keys_with_right_children) {
	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 0 );

	ASSERT_TRUE( internal.put(KEY_02, NODE_02) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( internal.keys_with_right_children().min_key(), KEY_02 ) );

	ASSERT_TRUE( internal.put(KEY_01, NODE_01) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 2 );
	EXPECT_TRUE( KEY_MATCHES( internal.keys_with_right_children().min_key(), KEY_01 KEY_02 ) );

	ASSERT_TRUE( internal.put(KEY_03, NODE_03) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 3 );
	EXPECT_TRUE( KEY_MATCHES( internal.keys_with_right_children().min_key(), KEY_01 KEY_02 KEY_03 ) );

	tree_node_t * node = NULL;

	ASSERT_TRUE( internal.del(KEY_01, &node) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 2 );
	EXPECT_TRUE( KEY_MATCHES( internal.keys_with_right_children().min_key(), KEY_02 KEY_03 ) );

	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( internal.keys_with_right_children().min_key(), KEY_03 ) );

	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( internal.keys_with_right_children().key_count() == 0 );
	EXPECT_TRUE( internal.keys_with_right_children().min_key() == NULL );
}

TEST_F(TreeInternalNodeTest, left_child) {
	tree_internal_node_t internal(TREE_KEY_LEN);

	tree_internal_node_t left_child(TREE_KEY_LEN);

	(void)internal.set_left_child( & left_child );

	EXPECT_TRUE( left_child.parent() == & internal);

}

// Test put/del/find_serving_node where an internal node's child is an internal node.
TEST_F(TreeInternalNodeTest, put_del_find_serving_internal_node) {

	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	tree_internal_node_t left_child(TREE_KEY_LEN);

	(void)internal.set_left_child( & left_child );

	tree_node_t * node = NULL;
	// Finding a serving node without any key in the internal node - should return the left child node.
	{
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_02, NODE_02) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_04 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_04, NODE_04) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	tree_leaf_node_t new_node_02(TREE_KEY_LEN); tree_leaf_node_t * NEW_NODE_02 = & new_node_02;

	// put : replace an existing key
	ASSERT_TRUE( internal.put(KEY_02, NEW_NODE_02) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	// del : del a non-existing key
	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( node == NULL );

	// del : del a existing key.
	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );


	// test find_serving_node
	{
		// KEY_01, KEY_02, KEY_03 is less than KEY_04. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	ASSERT_TRUE( internal.del(KEY_04, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

	// find_serving_node
	{
		// No key exists in the internal node. All keys should be served by the left_child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

}

// Test put/del/find_serving_node where an internal node's child is a leaf node.
TEST_F(TreeInternalNodeTest, put_del_find_serving_leaf_node) {

	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	tree_leaf_node_t left_child(TREE_KEY_LEN);
	tree_leaf_node_t leaf_02(TREE_KEY_LEN); tree_leaf_node_t * LEAF_02 = & leaf_02;
	tree_leaf_node_t leaf_04(TREE_KEY_LEN); tree_leaf_node_t * LEAF_04 = & leaf_04;


	(void)internal.set_left_child( & left_child );

	tree_node_t * node = NULL;
	// Finding a serving node without any key in the internal node - should return the left child node.
	{
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_02, LEAF_02) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_04 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_04, LEAF_04) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	tree_leaf_node_t new_leaf_02(TREE_KEY_LEN); tree_leaf_node_t * NEW_LEAF_02 = & new_leaf_02;

	// put : replace an existing key
	ASSERT_TRUE( internal.put(KEY_02, NEW_LEAF_02) == NLDB_OK );

	// test find_serving_node
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	// del : del a non-existing key
	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( node == NULL );

	// del : del a existing key.
	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );


	// test find_serving_node
	{
		// KEY_01, KEY_02, KEY_03 is less than KEY_04. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	ASSERT_TRUE( internal.del(KEY_04, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

	// find_serving_node
	{
		// No key exists in the internal node. All keys should be served by the left_child.
		ASSERT_TRUE( internal.find_serving_node(KEY_01, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_02, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_03, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_04, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node(KEY_05, &node) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

}

TEST_F(TreeInternalNodeTest, merge_with) {
	tree_internal_node_t internal1(TREE_KEY_LEN);
	tree_internal_node_t internal2(TREE_KEY_LEN);

	// merge_with is not supported yet.
	ASSERT_TRUE( internal1.merge_with(&internal2) == NLDB_ERROR_UNSUPPORTED_FEATURE );
}

// test split function where the children are internal nodes
TEST_F(TreeInternalNodeTest, split_with_two_keys_where_children_are_internal_nodes) {
	tree_internal_node_t org_internal(TREE_KEY_LEN);

	tree_internal_node_t left_child(TREE_KEY_LEN);

	(void) org_internal.set_left_child( &left_child );

	// put three keys
	ASSERT_TRUE( org_internal.put(KEY_03, NODE_03) == NLDB_OK );
	ASSERT_TRUE( org_internal.put(KEY_01, NODE_01) == NLDB_OK );
	ASSERT_TRUE( org_internal.put(KEY_05, NODE_05) == NLDB_OK );

	// the original internal node should have KEY_01, KEY_03, KEY_05
	EXPECT_TRUE( org_internal.keys_with_right_children().key_count() == 3 );
	EXPECT_TRUE( KEY_MATCHES( org_internal.keys_with_right_children().min_key(), KEY_01 KEY_03 KEY_05) );

	void * mid_key;
	tree_internal_node_t * new_right_internal;
	ASSERT_TRUE( org_internal.split( &new_right_internal, &mid_key ) == NLDB_OK );
	EXPECT_TRUE( KEY_03_MATCHES( mid_key ) );

	// the original internal node should have KEY_01
	EXPECT_TRUE( org_internal.keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( org_internal.keys_with_right_children().min_key(), KEY_01 ) );

	// the new right internal node should have KEY_05
	EXPECT_TRUE( new_right_internal->keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( new_right_internal->keys_with_right_children().min_key(), KEY_05 ) );

}

// test split function where the children are leaf nodes
TEST_F(TreeInternalNodeTest, split_with_two_keys_where_children_are_leaf_nodes) {
	tree_internal_node_t org_internal(TREE_KEY_LEN);

	tree_leaf_node_t left_child(TREE_KEY_LEN);
	tree_leaf_node_t leaf_01(TREE_KEY_LEN); tree_leaf_node_t * LEAF_01 = & leaf_01;
	tree_leaf_node_t leaf_03(TREE_KEY_LEN); tree_leaf_node_t * LEAF_03 = & leaf_03;
	tree_leaf_node_t leaf_05(TREE_KEY_LEN); tree_leaf_node_t * LEAF_05 = & leaf_05;

	(void) org_internal.set_left_child( &left_child );
	// put three keys
	ASSERT_TRUE( org_internal.put(KEY_03, LEAF_03) == NLDB_OK );
	ASSERT_TRUE( org_internal.put(KEY_01, LEAF_01) == NLDB_OK );
	ASSERT_TRUE( org_internal.put(KEY_05, LEAF_05) == NLDB_OK );

	// the original internal node should have KEY_01, KEY_03, KEY_05
	EXPECT_TRUE( org_internal.keys_with_right_children().key_count() == 3 );
	EXPECT_TRUE( KEY_MATCHES( org_internal.keys_with_right_children().min_key(), KEY_01 KEY_03 KEY_05) );

	void * mid_key;
	tree_internal_node_t * new_right_internal;
	ASSERT_TRUE( org_internal.split( &new_right_internal, &mid_key ) == NLDB_OK );
	EXPECT_TRUE( KEY_03_MATCHES( mid_key ) );

	// the original internal node should have KEY_01
	EXPECT_TRUE( org_internal.keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( org_internal.keys_with_right_children().min_key(), KEY_01 ) );

	// the new right internal node should have KEY_05
	EXPECT_TRUE( new_right_internal->keys_with_right_children().key_count() == 1 );
	EXPECT_TRUE( KEY_MATCHES( new_right_internal->keys_with_right_children().min_key(), KEY_05 ) );

}

