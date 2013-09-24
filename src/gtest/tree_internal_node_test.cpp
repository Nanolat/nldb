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

// Test put/del/find_serving_node_by_key where an internal node's child is an internal node.
TEST_F(TreeInternalNodeTest, put_del_find_serving_internal_node) {

	tree_internal_node_t internal(TREE_KEY_LEN);

	ASSERT_TRUE( TREE_KEYS_PER_NODE == 3 );

	tree_internal_node_t left_child(TREE_KEY_LEN);

	(void)internal.set_left_child( & left_child );

	tree_node_t * node = NULL;
	// Finding a serving node without any key in the internal node - should return the left child node.
	{
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_02, NODE_02) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_04 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_04, NODE_04) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_02) );

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	tree_leaf_node_t new_node_02(TREE_KEY_LEN); tree_leaf_node_t * NEW_NODE_02 = & new_node_02;

	// put : replace an existing key
	ASSERT_TRUE( internal.put(KEY_02, NEW_NODE_02) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the NODE_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	// del : del a non-existing key
	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( node == NULL );

	// del : del a existing key.
	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NEW_NODE_02) );


	// test find_serving_node_by_key
	{
		// KEY_01, KEY_02, KEY_03 is less than KEY_04. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_04 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

		// KEY_05 should be served by the NODE_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );
	}

	ASSERT_TRUE( internal.del(KEY_04, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NODE_04) );

	// find_serving_node_by_key
	{
		// No key exists in the internal node. All keys should be served by the left_child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

}

// Test put/del/find_serving_node_by_key where an internal node's child is a leaf node.
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
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_02, LEAF_02) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_04 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );
	}

	// put : put a new key
	ASSERT_TRUE( internal.put(KEY_04, LEAF_04) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_02) );

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	tree_leaf_node_t new_leaf_02(TREE_KEY_LEN); tree_leaf_node_t * NEW_LEAF_02 = & new_leaf_02;

	// put : replace an existing key
	ASSERT_TRUE( internal.put(KEY_02, NEW_LEAF_02) == NLDB_OK );

	// test find_serving_node_by_key
	{
		// KEY_01 is less than KEY_02. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_02 should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );

		// KEY_03 is greater than KEY_02, so it should be served by the LEAF_02.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	// del : del a non-existing key
	ASSERT_TRUE( internal.del(KEY_03, &node) == NLDB_OK );
	EXPECT_TRUE( node == NULL );

	// del : del a existing key.
	ASSERT_TRUE( internal.del(KEY_02, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, NEW_LEAF_02) );


	// test find_serving_node_by_key
	{
		// KEY_01, KEY_02, KEY_03 is less than KEY_04. So KEY_01 should be served by the left child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		// KEY_04 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

		// KEY_05 should be served by the LEAF_04.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );
	}

	ASSERT_TRUE( internal.del(KEY_04, &node) == NLDB_OK );
	EXPECT_TRUE( NODE_MATCHES(node, LEAF_04) );

	// find_serving_node_by_key
	{
		// No key exists in the internal node. All keys should be served by the left_child.
		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_01, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_02, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_03, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_04, &node, NULL) == NLDB_OK );
		EXPECT_TRUE( node == & left_child);

		ASSERT_TRUE( internal.find_serving_node_by_key(KEY_05, &node, NULL) == NLDB_OK );
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

