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
