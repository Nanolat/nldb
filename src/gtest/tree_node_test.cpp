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

class TreeNodeTest : public testing::Test  {
  protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TreeNodeTest, is_internal_node) {
	tree_node_t internal_node(tree_node_t::INTERNAL_NODE_MAGIC);
	ASSERT_TRUE( internal_node.is_internal_node() );
}

TEST_F(TreeNodeTest, is_leaf_node) {
	tree_node_t leaf_node(tree_node_t::LEAF_NODE_MAGIC);
	ASSERT_TRUE( leaf_node.is_leaf_node() );
}

TEST_F(TreeNodeTest, is_parent) {
	// 1. Root Node
	tree_node_t root_node(tree_node_t::INTERNAL_NODE_MAGIC);
	ASSERT_TRUE( root_node.parent() == NULL );

	// 2. Internal Node's parent := Root Node
	tree_node_t internal_node(tree_node_t::INTERNAL_NODE_MAGIC);
	ASSERT_TRUE( internal_node.parent() == NULL );

	(void) internal_node.set_parent( & root_node );
	ASSERT_TRUE( internal_node.parent() == &root_node );

	(void) internal_node.set_parent( NULL );
	ASSERT_TRUE( internal_node.parent() == NULL );

	// 3. Leaf Node's parent := Internal Node
	tree_node_t leaf_node(tree_node_t::LEAF_NODE_MAGIC);
	ASSERT_TRUE( leaf_node.parent() == NULL );

	(void) leaf_node.set_parent( & internal_node );
	ASSERT_TRUE( leaf_node.parent() == &internal_node );

	(void) leaf_node.set_parent( NULL );
	ASSERT_TRUE( leaf_node.parent() == NULL );

}
