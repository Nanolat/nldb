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
