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

#ifndef NLDB_ARRAY_TREE_H_
#define NLDB_ARRAY_TREE_H_ (1)

#include "nldb_sorted_array.h"

// for memcmp
#include <string.h>

// for debugging printf
#include <stdio.h>

#include <txbase/tx_assert.h>

const int NLDB_TREE_MIN_KEY_SIZE = 1;

#include "nldb_internal.h"
/*
 * nldb_array_tree : a balanced tree whose nodes have sorted array of fixed size keys.
 */
template<int key_space_size> class nldb_array_tree {
protected:
	class node_t {
	public :
		// The magic value to check if the node memory is not corrupt
		typedef enum NODE_MAGIC_VALUE {
			INTERNAL_NODE_MAGIC=0xcafecafe,
			LEAF_NODE_MAGIC=0xcafebebe
		}NODE_MAGIC_VALUE;
	protected :
		/*! The number of keys under this node.
		 * For example, the root node has the total number of keys.
		 * An internal node has total number of keys in all leaf nodes under it.
		 * A leaf node has total number of keys in the node.
		 */
		nldb_order_t key_count_;
		// The parent node.
		node_t * parent_;
		// Magic value.
		NODE_MAGIC_VALUE magic_;
	public :
		node_t(NODE_MAGIC_VALUE magic) {
			tx_debug_assert(magic == INTERNAL_NODE_MAGIC || magic == LEAF_NODE_MAGIC);
			key_count_ = 0;
			parent_ = NULL;
			magic_ = magic;
		}
		virtual ~node_t() {}

		#define has_valid_magic() (magic_ == INTERNAL_NODE_MAGIC || magic_ == LEAF_NODE_MAGIC)

		inline nldb_order_t key_count() {
			tx_debug_assert( has_valid_magic() );

			return key_count_;
		}

		inline void set_key_count(const nldb_order_t key_count) {
			tx_debug_assert( has_valid_magic() );

			key_count_ = key_count;
		}

		inline bool is_internal_node() const
		{
			tx_debug_assert( has_valid_magic() );

			return (magic_ == INTERNAL_NODE_MAGIC) ? true : false;
		}

		inline bool is_leaf_node() const
		{
			tx_debug_assert( has_valid_magic() );

			return (magic_ == LEAF_NODE_MAGIC) ? true : false;
		}


		inline node_t * parent() {
			tx_debug_assert( has_valid_magic() );

			return parent_;
		}

		inline void set_parent( node_t * parent ) {
			tx_debug_assert( has_valid_magic() );

			parent_ = parent;
		}
	};

	class leaf_node_t : public node_t
	{
	protected:
		// The sorted keys in an array. Each key has a mapping from a key to pointer of the value.
		nldb_sorted_array<key_space_size> keys_with_values_;

	public:
		// The next sibling node
		leaf_node_t * next_;
		// The previous sibling
		leaf_node_t * prev_;

		leaf_node_t(const key_length_t & key_length) : node_t( node_t::LEAF_NODE_MAGIC )
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( key_length > 0 );

			// The internal node should be able to store at least three keys.
			tx_debug_assert( key_space_size/key_length >= 2 );

			keys_with_values_.init(key_length);

			next_ = NULL;
			prev_ = NULL;
		}
		virtual ~leaf_node_t()
		{
			tx_debug_assert( node_t::is_leaf_node() );
		}

		bool is_empty() const
		{
			tx_debug_assert( node_t::is_leaf_node() );

			return keys_with_values_.is_empty();
		}

		bool is_full() const
		{
			tx_debug_assert( node_t::is_leaf_node() );

			return keys_with_values_.is_full();
		}

		// Return NULL if no key exists in this node.
		// Otherwise return the minimum key in this node.
		const void * min_key() const
		{
			tx_debug_assert( node_t::is_leaf_node() );

			return keys_with_values_.min_key();
		}

		nldb_sorted_array<key_space_size> & keys_with_values()
		{
			tx_debug_assert( node_t::is_leaf_node() );

			return keys_with_values_;
		}

		nldb_rc_t put (const void * key, const void * value, bool * isReplaced)
		{
			tx_debug_assert( node_t::is_leaf_node() );
			tx_debug_assert( key != NULL );
			tx_debug_assert( value != NULL );
			tx_debug_assert( isReplaced != NULL);

			int key_count_before_put = keys_with_values_.key_count();

			nldb_rc_t rc = keys_with_values_.put(key, value);
			if (rc) return rc;

			if (key_count_before_put == keys_with_values_.key_count()) {
				*isReplaced = true;
			} else {
				*isReplaced = false;
			}

			return NLDB_OK;
		}

		nldb_rc_t get(const void * key, void ** value, nldb_order_t * key_order ) const
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( key != NULL );
			tx_debug_assert( value != NULL );
			// key_order could be NULL if there is no need to get the order of the key.

			nldb_rc_t rc = keys_with_values_.get(key, value, key_order);
			if (rc) return rc;

			return NLDB_OK;
		}

		/*! Get the key_order th the key and data.
		 * @param key_order 1-based index to the key array.
		 */
		nldb_rc_t get(const nldb_order_t key_order, void ** key, void ** value ) const
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( key_order >= 1 );
			tx_debug_assert( key_order <= keys_with_values_.key_count() );
			tx_debug_assert( key != NULL );
			tx_debug_assert( value != NULL );
			// key_order could be NULL if there is no need to get the order of the key.

			nldb_rc_t rc = keys_with_values_.get(key_order, key, value);
			if (rc) return rc;

			return NLDB_OK;
		}

		nldb_rc_t del (const void * key, void ** value)
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( key != NULL );
			tx_debug_assert( value != NULL );

			nldb_rc_t rc = keys_with_values_.del(key, value);
			if (rc) return rc;

			return NLDB_OK;
		}

		nldb_rc_t merge_with(leaf_node_t * node)
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( node != NULL );

			// Not implemented yet. Merging nodes is P2. We will implement the feature after the first release of NLDB.
			return NLDB_ERROR_UNSUPPORTED_FEATURE;
		}

		nldb_rc_t split(leaf_node_t ** right_node )
		{
			tx_debug_assert( node_t::is_leaf_node() );

			tx_debug_assert( right_node != NULL );

			// A leaf node should have at least two keys before the split operation
			tx_debug_assert( keys_with_values_.key_count() >= 2 );

			leaf_node_t * new_node = node_factory::new_leaf_node(keys_with_values_.key_length());

			// Move half of keys in this node to "node".
			nldb_rc_t rc = keys_with_values_.split( & new_node->keys_with_values() );
			if(rc) return rc;

			// Check link consistency
			tx_debug_assert(this->prev_ == NULL || this->prev_->next_ == this);
			tx_debug_assert(this->next_ == NULL || this->next_->prev_ == this);

			// Maintain the double linked list on the leaf nodes.
			new_node->prev_ = this;
			new_node->next_ = this->next_;

			// The parent is not set yet.
			// It can be set after the min key of the new node is put into a parent node.
			// However, we can't guarantee that node->parent is new_node's parent
			// because the node->parent may split while putting the min key of the new node into it.

			if ( this->next_ )
				this->next_->prev_ = new_node;

			this->next_ = new_node;

			// Adjust key count.
			this->key_count_ = keys_with_values_.key_count();
			new_node->set_key_count( new_node->keys_with_values().key_count() );

			*right_node = new_node;

			return NLDB_OK;
		}

	};

	class internal_node_t : public node_t
	{
	protected:
		// The sorted keys in an array. Each key has a mapping from a key to the right children node.
		nldb_sorted_array<key_space_size> keys_with_right_children_;

		// The child node which has keys less than than the first key.
		node_t * left_child_;
	public :
		internal_node_t(const key_length_t & key_length) : node_t( node_t::INTERNAL_NODE_MAGIC )
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( key_length > 0 );

			// The internal node should be able to store at least three keys.
			tx_debug_assert( key_space_size/key_length >= 3 );

			keys_with_right_children_.init(key_length );
		}
		virtual ~internal_node_t()
		{
			tx_debug_assert( node_t::is_internal_node() );
		}

		void set_left_child(node_t * left_child)
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( left_child != NULL );

			left_child_ = left_child;

			left_child_->set_parent(this);
		}

		node_t * left_child() const
		{
			tx_debug_assert( node_t::is_internal_node() );

			return left_child_;
		}

		bool is_empty() const
		{
			tx_debug_assert( node_t::is_internal_node() );

			return keys_with_right_children_.is_empty();
		}

		bool is_full() const
		{
			tx_debug_assert( node_t::is_internal_node() );

			return keys_with_right_children_.is_full();
		}

		const void * min_key() const
		{
			tx_debug_assert( node_t::is_internal_node() );

			return keys_with_right_children_.min_key();
		}

		nldb_sorted_array<key_space_size> & keys_with_right_children()
		{
			tx_debug_assert( node_t::is_internal_node() );

			return keys_with_right_children_;
		}

		nldb_rc_t put (const void * key, node_t * node )
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( key != NULL );
			tx_debug_assert( node != NULL );

			nldb_rc_t rc = keys_with_right_children_.put(key, node);
			if (rc) return rc;

			node->set_parent(this);

			return NLDB_OK;
		}

		/*! Get the number of all keys under the left siblings of a child node at the given position.
		 * @node_position The position of the child node in keys_with_right_children_.
		 * @o_all_keys_before_position (out) the total number of all keys under the left siblings of the given child node.
		 */
		nldb_rc_t count_all_keys_before_position(int node_position, nldb_order_t * o_all_keys_before_position) const {
			tx_debug_assert( o_all_keys_before_position );

			int total_key_count_before_the_position = 0;
			if (node_position >= 0) {
				total_key_count_before_the_position += left_child_->key_count();
			}

			for (int pos = 0; pos < node_position; pos ++) {
				// Get each child node before the serving node.
				node_t * child = (node_t *) keys_with_right_children_.get_data(pos);

				tx_debug_assert(child);

				total_key_count_before_the_position += child->key_count();
			}
			*o_all_keys_before_position = total_key_count_before_the_position;

			return NLDB_OK;
		}

		/*! Count all keys in leaf nodes under the current internal node.
		 * This is necessary to count keys after the split of an internal node.
		 */
		nldb_rc_t count_all_keys_in_leaf_nodes(nldb_order_t * o_key_count) {
			tx_debug_assert(o_key_count);

			int after_the_last_node_position = keys_with_right_children_.key_count();
			nldb_order_t all_key_count;
			nldb_rc_t rc = count_all_keys_before_position(after_the_last_node_position, &all_key_count);
			if (rc) return rc;

			*o_key_count = all_key_count;

			return NLDB_OK;
		}

		/*! Return the subtree which should serve the given key.
		 * Note that the key might not exist in the subtree.
		 * Set the root of the subtree to *node.
		 *
		 * @param key The key should be served by the found node.
		 * @param node The found serving node.
		 * @param key_count_before_the_serving_node The total number of keys in leaf nodes before the serving node.
         *                                          For the details, see nldb_sorted_array::find_last_le_key
         *
		 * @param key The key to find.
		 * @param data (out) The found serving node.
		 * @param key_count_before_the_serving_node (out) The total number of keys in leaf nodes under the left siblings of the serving node.
		 *
		 * Assume that an internal node has key1, key2, key3.
		 * The serving node for keys in [-oo, key1)  is node1. *key_count_before_the_serving_node is set to 0.
		 * The serving node for keys in [key1, key2) is node2. *key_count_before_the_serving_node is set to node1->key_count().
		 * The serving node for keys in [key2, +oo)  is node3. *key_count_before_the_serving_node is set to node1->key_count() + node2->key_count().
		 *
		 *       +----<key1>---<key2>
		 *       |      |        |
		 *   <node1>  <node2>  <node3>
		 *
	     */
		nldb_rc_t find_serving_node_by_key (const void * key, node_t ** node, nldb_order_t * o_key_count_before_the_serving_node ) const
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( key != NULL );
			tx_debug_assert( node != NULL );

			int node_position;

			// find the node that is mapped with the last less than or equal to the given key.
			nldb_rc_t rc = keys_with_right_children_.find_last_le_key(key, (void**)node, &node_position);
			if (rc) return rc;

			// keys_with_right_children_ does not have the serving node for the key.
			// IOW, all keys in keys_with_right_children_ are greater than the given key.
			// In this case, the left child of this node should serve the key in this case.
			if ( *node == NULL )
				*node = left_child_;

			if (o_key_count_before_the_serving_node) {
				rc = count_all_keys_before_position(node_position, o_key_count_before_the_serving_node);
				if (rc) return rc;
			}

			return NLDB_OK;
		}

		/*! Return the subtree which should serve a key whose ranking is key_order.
		 * @param key_order The serving node should serve a key whose rank is key_order.
		 * @param node (out) The found serving node.
		 * @param key_count_before_the_serving_node (out) The total number of keys in leaf nodes under the left siblings of the serving node.
		 *
		 * Assume that an internal node has key1, key2, key3.
		 * The serving node for key_order in (-oo, node1.key_count] is node1. *key_count_before_the_serving_node is set to 0.
		 * The serving node for key_order in (node1.key_count, node1.key_count + node2.key_count] is node2. *key_count_before_the_serving_node is set to node1->key_count().
		 * The serving node for key_order in (node1.key_count + node2.key_count, node1.key_count + node2.key_count + node3.key_count] is node3.
		 * In this case, *key_count_before_the_serving_node is set to node1->key_count() + node2->key_count().
		 *
		 *       +----<key1>---<key2>
		 *       |       |        |
		 *   <node1>  <node2>  <node3>
		 *
		 */
		nldb_rc_t find_serving_node_by_order (const nldb_order_t key_order, node_t ** o_node, nldb_order_t * o_key_count_before_the_serving_node) const
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( key_order > 0 );
			tx_debug_assert( o_node != NULL );

			nldb_order_t key_count_left = key_order;

			node_t * serving_node = NULL;

			int node_position;

			// Does the left child contain the key whose rank is key_order?
			if (key_count_left <= left_child_->key_count() ) {
				serving_node = left_child_;
				node_position = -1;
			}
			else
			{
				key_count_left -= left_child_->key_count();

				for ( node_position = 0; node_position < keys_with_right_children_.key_count(); node_position++ ) {
					node_t * child = (node_t*) keys_with_right_children_.get_data( node_position );

					tx_debug_assert(child);

					if (key_count_left <= child->key_count() ) {
						serving_node = child;
						break;
					}

					key_count_left -= child->key_count();
				}
			}

			// If serving_node is NULL, it means that the key_order was greater than the total keys under the node.
			tx_assert(serving_node);

			*o_node = serving_node;

			tx_debug_assert( key_order >= key_count_left );

			// Now key_count_left becomes the rank of the key in the subtree whose root is the serving node.
			// Simply subtract key_count_left from the original rank of the key
			// to get the number of all keys under the left siblings of the serving node.
			*o_key_count_before_the_serving_node = key_order - key_count_left;

			return NLDB_OK;
		}


		nldb_rc_t del (const void * key, node_t ** deleted_node )
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( key != NULL );
			tx_debug_assert( deleted_node != NULL );

			node_t * node;

			nldb_rc_t rc = keys_with_right_children_.del(key, (void**)&node);
			if (rc) return rc;

			if (node)
				node->set_parent(NULL);

			*deleted_node = node;

			return NLDB_OK;
		}

		nldb_rc_t merge_with(internal_node_t * node)
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( node != NULL );

			// Not implemented yet. Merging nodes is P2. We will implement the feature after the first release of NLDB.
			return NLDB_ERROR_UNSUPPORTED_FEATURE;
		}

		nldb_rc_t split(internal_node_t ** right_node, void ** mid_key )
		{
			tx_debug_assert( node_t::is_internal_node() );

			tx_debug_assert( right_node != NULL );
			tx_debug_assert( mid_key != NULL );

			internal_node_t * new_node = node_factory::new_internal_node(keys_with_right_children_.key_length());

			// An internal node should have at least two keys before the split operation
			// The original node should have at least three keys.
			// One for the left node, one for the right node, and the last one for the key in between them.
			tx_debug_assert( keys_with_right_children_.key_count() >= 3);

			// Move half of keys in this node to "node".
			nldb_rc_t rc = keys_with_right_children_.split( & new_node->keys_with_right_children() );
			if (rc) return rc;

			// Update parent of the children who moved to new_node.
			{
				typename nldb_sorted_array<key_space_size>::iterator_t iter;
				rc = new_node->keys_with_right_children().iter_forward(&iter);
				if (rc) return rc;

				while(1) {
					void * dummy_key = NULL;
					node_t * child   = NULL;
					rc = new_node->keys_with_right_children().iter_next(iter, &dummy_key, (void**)&child);
					if (rc) return rc;

					if (child == NULL) break;

					(void) child->set_parent(new_node);
				}
			}

			// The key in the middle between the two split nodes.
			void * mid_key_node = NULL;
			rc = keys_with_right_children_.remove_max_key(mid_key, & mid_key_node);
			if (rc) return rc;

			// Even after removing the maximum key, the node should have at least a key.
			tx_assert(keys_with_right_children_.key_count() > 0 );

			// Set the node attached with the mid key to the left child of the new split node.
			(void)new_node->set_left_child((node_t*)mid_key_node);

			// Set key count
			{
				nldb_order_t key_count ;
				nldb_rc_t rc = new_node->count_all_keys_in_leaf_nodes(&key_count);
				if (rc) return rc;
				new_node->set_key_count(key_count);

				rc = this->count_all_keys_in_leaf_nodes(&key_count);
				if (rc) return rc;
				this->set_key_count(key_count);
			}

			*right_node = new_node;

			return NLDB_OK;
		}
	};

	class node_factory {
    public :
		static leaf_node_t * new_leaf_node(const key_length_t & key_length)
		{
			tx_debug_assert( key_length > 0 );

			leaf_node_t * p = new leaf_node_t(key_length);
			return p;
		}
		static void return_leaf_node(leaf_node_t * leaf_node)
		{
			tx_debug_assert( leaf_node != NULL );

			delete leaf_node;
		}
		static internal_node_t * new_internal_node(const key_length_t & key_length)
		{
			tx_debug_assert( key_length > 0 );

			internal_node_t * p = new internal_node_t(key_length);
			return p;
		}
		static void return_internal_node(internal_node_t * internal_node)
		{
			tx_debug_assert( internal_node != NULL );

			delete internal_node;
		}
	};

	inline int compare_keys(void * key1, void * key2) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key1 != NULL );
		tx_debug_assert( key2 != NULL );

		return memcmp(key1, key2, key_length_);
	}

	inline int compare_keys(const void * key1, void * key2) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key1 != NULL );
		tx_debug_assert( key2 != NULL );

		return memcmp(key1, key2, key_length_);
	}

	inline int compare_keys(void * key1, const void * key2) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key1 != NULL );
		tx_debug_assert( key2 != NULL );

		return memcmp(key1, key2, key_length_);
	}

	inline int compare_keys(const void * key1, const void * key2) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key1 != NULL );
		tx_debug_assert( key2 != NULL );

		return memcmp(key1, key2, key_length_);
	}

	/*! Find the leaf node that has the given key.
	 * @param key_order the order of the key in the tree.
	 * @param node the found node which has key_order the key.
	 * @param key_count_before_the_found_node the number of keys stored in all leaf nodes before the found node.
	 *        For example, if the found node were <leaf4>, all keys in <leaf1>, <leaf2>, <leaf3> are less than the min key of <leaf4>.
	 *        *key_count_before_the_found_node is set to the number of all keys in <leaf1>, <leaf2>, <leaf3>.
	 *        <leaf1> - <leaf2> - <leaf3> - <leaf4> - <leaf5>
	 *        This parameter can be NULL if there is no need to get the key count.
	 */

	nldb_rc_t find_leaf_node(const void * key, leaf_node_t ** node, nldb_order_t * o_key_count_before_the_found_node) const {
		tx_debug_assert( is_initialized() );

		tx_debug_assert( key != NULL );
		tx_debug_assert( node != NULL );

		nldb_order_t total_key_count_before_the_leaf_node = 0;

		node_t * n;
		for (n = root_node_;
			 n->is_internal_node();
			 )
		{
			internal_node_t * internal_node = (internal_node_t*) n ;

			if (o_key_count_before_the_found_node) { // Do we need to get the key order as well?
				nldb_order_t all_key_count_before_n;
				nldb_rc_t rc = internal_node->find_serving_node_by_key(key, &n, &all_key_count_before_n);
				if (rc) return rc;

				total_key_count_before_the_leaf_node += all_key_count_before_n;
			} else { // No need to get the key order.
				nldb_rc_t rc = internal_node->find_serving_node_by_key(key, &n, NULL);
				if (rc) return rc;
			}

			tx_debug_assert(n);

		}

		tx_debug_assert( n->is_leaf_node() );

		*node = (leaf_node_t*) n;

		if (o_key_count_before_the_found_node) {
			*o_key_count_before_the_found_node = total_key_count_before_the_leaf_node;
		}

		return NLDB_OK;
	}

	/*! Find the leaf node that has the key_order th key.
	 * @param key_order the order of the key in the tree.
	 * @param node the found node which has key_order the key.
	 * @param key_count_before_the_found_node the number of keys stored in all leaf nodes before the found node.
	 *        For example, if the found node were <leaf4>, all keys in <leaf1>, <leaf2>, <leaf3> are less than the min key of <leaf4>.
	 *        *key_count_before_the_found_node is set to the number of all keys in <leaf1>, <leaf2>, <leaf3>.
	 *        <leaf1> - <leaf2> - <leaf3> - <leaf4> - <leaf5>
	 *        this parameter should NOT be NULL.
	 */
	nldb_rc_t find_leaf_node(const nldb_order_t key_order, leaf_node_t ** node, nldb_order_t * o_key_count_before_the_found_node) const {
		tx_debug_assert( is_initialized() );

		tx_debug_assert( key_order >= 1 );
		tx_debug_assert( key_order <= root_node_->key_count() );
		tx_debug_assert( node != NULL );
		tx_debug_assert( o_key_count_before_the_found_node );

		nldb_order_t total_key_count_before_the_leaf_node = 0;

		nldb_order_t key_count_left = key_order;

		node_t * n;
		for (n = root_node_;
			 n->is_internal_node();
			 )
		{
			internal_node_t * internal_node = (internal_node_t*) n ;

			nldb_order_t all_key_count_before_n;

			nldb_rc_t rc = internal_node->find_serving_node_by_order( key_count_left, &n, &all_key_count_before_n );
			if (rc) return rc;

			tx_debug_assert(n);

			key_count_left -= all_key_count_before_n;
			tx_debug_assert( key_count_left > 0 );

			total_key_count_before_the_leaf_node += all_key_count_before_n;

		}

		tx_debug_assert( n->is_leaf_node() );

		*node = (leaf_node_t*) n;

		*o_key_count_before_the_found_node = total_key_count_before_the_leaf_node;

		return NLDB_OK;
	}

	/*! Put a key into an internal node. Let the key point to another node.
	 * @param node The internal node where the key is put.
	 * @param key The key to put into the internal node.
	 * @param key_node The key node that the key will point to.
	 * @param o_unsplit_top_node (out) the top-most node that were not split.
	 *                           This will be used as a fence node for propagating the increment of the key count in the right split leaf node,
	 *                           which was not included when we recalcuate key count of each split internal node.
	 */
	nldb_rc_t put_to_internal_node(internal_node_t * node, const void * key, node_t * key_node, internal_node_t ** o_unsplit_top_node)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( node );
		tx_debug_assert( key );
		tx_debug_assert( key_node );
		tx_debug_assert( o_unsplit_top_node );

		if ( node->is_full() )
		{
#if !defined(NDEBUG)
			nldb_order_t key_count_before_split = node->key_count();
#endif
			internal_node_t * right_node = NULL;

			// The key in the middle between the original node and the right node.
			// The key was removed from the original node.
			// right_node node not keep the key, but its left_child_ points to the node of the removed key.
			void * mid_key = NULL;

			nldb_rc_t rc = node->split( &right_node, & mid_key );
			if (rc) return rc;

			tx_debug_assert(right_node);

			// At this point, mid_key is a pointer to a key in the key_space_ of the node.
			// If we put the 'key' parameter in the node, the mid_key can be changed to point to another key.
			// So, we first modify parent nodes, before puting the 'key' parameter in either node or right_node.
			if ( node->parent() ) // not a root node
			{
				rc = put_to_internal_node( (internal_node_t*) node->parent(), mid_key, right_node, o_unsplit_top_node);
				if (rc) return rc;

				// Keys in the right node are not counted when we recalculate the key count of all split nodes in put_to_internal_node.
				// Add the number of keys in right_node to all split nodes above the right_node.
				// The fence node, *o_unsplit_top_node can be NULL if all nodes up to the root node was split.
				rc = propagate_key_count_increment(right_node->parent(), *o_unsplit_top_node /*fence*/, right_node->key_count() );
				if (rc) return rc;
			}
			else // root node
			{
				// create a new root node
				internal_node_t * new_root_node = node_factory::new_internal_node(key_length_);
				// Set the old root node as the left child of the new root node.
				// node->parent_ is set to new_root_node by this code.
				new_root_node->set_left_child(node);
				// Set the right node as the 2nd child of the new root node.
				new_root_node->put(mid_key, right_node);

				// Because the keys in the newly split leaf node is not counted,
				// the original key count is always greater than sum of key counts in the two split nodes.
				tx_debug_assert(key_count_before_split > node->key_count() + right_node->key_count() );

				// Set key count of the new root node
				new_root_node->set_key_count( node->key_count() + right_node->key_count() );
				// Yes! the new root node is elected!
				root_node_ = new_root_node;

				// The root node was split. Set the fence node to NULL
				// so that missing key count can be propagated from the newly split leaf node up to the root node.
				*o_unsplit_top_node = NULL;
			}

			int cmp = compare_keys(key, mid_key);

			if ( cmp < 0 )
			{
				// put the key into the parent node. This code also sets the key_node's parent.
				rc = node->put(key, key_node);
				if (rc) return rc;
			}
			else if (cmp > 0 )
			{
				// put the key into the parent node. This code also sets the key_node's parent.
				rc = right_node->put(key, key_node);
				if (rc) return rc;
			}
			else // cmp == 0
			{
				// The mid_key is the key in the middle in the node to be splitted.
				// The key and key_node is from the child of this node, which is splitted before this function was called.
				// It does not make sense the key in the middle of the splitted child node exists in the parent(this) node.
				tx_assert(0);
			}
		}
		else
		{
			// put the key into the parent node. This code also sets the key_node's parent.
			nldb_rc_t rc = node->put(key, key_node);
			if (rc) return rc;

			// Ok, the node was not split, set it to *o_unsplit_top_node so that it can be used for the fence node for propagating
			// increment of key count for the missing keys that were not included while recalculating key count of each split internal node.
			*o_unsplit_top_node = node;
		}

		return NLDB_OK;
	}

	/*! Put a key into the given leaf node.
	 * @node The node where the key will be put.
	 * @key The key to put
	 * @value The value to put
	 * @o_key_existing_node The node that the key exists after the put operation.
	 *                      In case the original node is split, the key can be put into the newly created node.
	 *                      In this case, o_key_existing_node points to the newly created node.
	 * @isReplaced indicates if the key was replaced because the same key existed before.
	 *             We won't increase key count if this is true.
	 */
	nldb_rc_t put_to_leaf_node(leaf_node_t * node, const void * key, const void * value, leaf_node_t ** o_key_existing_node, bool *isReplaced)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( node != NULL );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );
		tx_debug_assert( o_key_existing_node != NULL );
		tx_debug_assert( isReplaced != NULL);

		if ( node->is_full() )
		{
			leaf_node_t * right_node = NULL;

#if !defined(NDEBUG)
			nldb_order_t key_count_before_split = node->key_count();
#endif

			nldb_rc_t rc = node->split( &right_node );
			if (rc) return rc;

			tx_debug_assert( right_node != NULL );

			// the keys in the split nodes should be equal to the key count before the split.
			tx_debug_assert( key_count_before_split == node->key_count() + right_node->key_count() );

			const void * mid_key = right_node->min_key();
			tx_debug_assert(mid_key);

			if ( compare_keys(key, mid_key) < 0 )
			{
				rc = node->put(key, value, isReplaced);
				if (rc) return rc;

				*o_key_existing_node = node;
			}
			else
			{
				rc = right_node->put(key,value, isReplaced);
				if (rc) return rc;

				*o_key_existing_node = right_node;
			}

			internal_node_t * unsplit_top_node;

			rc = put_to_internal_node( (internal_node_t*) node->parent(), mid_key, right_node, &unsplit_top_node);
			if (rc) return rc;

			// Keys in the right node are not counted when we recalculate the key count of all split nodes in put_to_internal_node.
			// Add the number of keys in right_node to all split nodes above the right_node.
			// unsplit_top_node can be NULL if all nodes up to the root node was split.
			rc = propagate_key_count_increment(right_node->parent(), unsplit_top_node /*fence*/, right_node->key_count() );
			if (rc) return rc;
		}
		else
		{
			nldb_rc_t rc = node->put(key, value, isReplaced);
			if (rc) return rc;

			*o_key_existing_node = node;
		}

		return NLDB_OK;
	}

	nldb_rc_t del_from_leaf_node(leaf_node_t * node, const void * key, void ** deleted_value)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( node != NULL );
		tx_debug_assert( key != NULL );
		tx_debug_assert( deleted_value != NULL );

		nldb_rc_t rc = node->del(key, deleted_value);
		if (rc) return rc;

		if ( *deleted_value == NULL)
		{
			return NLDB_OK;
		}

		return NLDB_OK;
	}

	/*! Starting from a node, increase the key_count counter in each node up any child of fence_node.
	 * By default fence_node is NULL, meaning to increase the key_count counter up to root node.
	 */
	nldb_rc_t propagate_key_count_increment(node_t * from_node, node_t * fence_node = NULL, nldb_order_t increment = 1) {
		tx_debug_assert(from_node);
//printf("propagating key increment up to %llX, %lld\n", fence_node, increment);
		for (node_t * n = from_node;
 		     n != fence_node;
			 n = n->parent()) {
//printf("adding key increment for node : %llX, %lld + %lld\n", n, n->key_count(), increment);
			n->set_key_count( n->key_count() + increment);
		}
		return NLDB_OK;
	}

	/*! Starting from a node, decrease the key_count counter in each node up any child of fence_node.
	 * By default fence_node is NULL, meaning to increase the key_count counter up to root node.
	 */
	nldb_rc_t propagate_key_count_decrement(node_t * from_node, node_t * fence_node = NULL, nldb_order_t decrement = 1) {
		tx_debug_assert(from_node);

		for (node_t * n = from_node;
 		     n != fence_node;
			 n = n->parent()) {
			n->set_key_count( n->key_count() - decrement);
		}
		return NLDB_OK;
	}

	bool initialized_;
	int  key_length_;

	internal_node_t * root_node_;

public:
	typedef struct iterator_t
	{
		// The node that has the key to iterate.
		leaf_node_t * current_node_;
		// The iterator within the sorted key array in the current_node_.
		typename nldb_sorted_array<key_space_size>::iterator_t key_iter_;
	} iterator_t;

	int key_length() {
		tx_debug_assert( key_length_ > 0 );

		return key_length_;
	}
	nldb_array_tree() {
		initialized_ = false;
	}
	virtual ~nldb_array_tree() {}

	bool is_initialized() const {
		return initialized_;
	}

	nldb_rc_t init(const key_length_t & key_length)
	{
		// the array tree object should not be initialized twice.
		tx_debug_assert( ! is_initialized() );

		key_length_ = key_length;

		// 1. Create the root node.
		root_node_ = node_factory::new_internal_node( key_length_ );

		// 2. Create a leaf node to add it as the left child of the root node.
		leaf_node_t * leaf = node_factory::new_leaf_node( key_length_ );

		// Set the leaf node as the left child of the root node.
		(void)root_node_->set_left_child(leaf);

		initialized_ = true;
		return NLDB_OK;
	}

	nldb_rc_t destroy()
	{
		initialized_ = false;

		return NLDB_OK;
	}

	nldb_rc_t put (const void * key, const void * value)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );

		bool isReplaced = false;
		leaf_node_t * leaf = NULL;
		leaf_node_t * key_existing_node = NULL;

		nldb_rc_t rc = find_leaf_node(key, &leaf, NULL /*key_count_before_the_found_node*/ );
		if (rc) return rc;

		// In case the leaf node was split, key_existing_node points to the new node.
		// Otherwise, it points to the leaf node.
		rc = put_to_leaf_node(leaf, key, value, &key_existing_node, &isReplaced);
		if (rc) return rc;

		if (!isReplaced) { // increase the key count only if the same key did not exist.
			// Increase the key_count counter in each node from key_existing_node up to the root node.
			rc = propagate_key_count_increment(key_existing_node);
			if (rc) return rc;
		}

		return NLDB_OK;
	}

	/*! Get the value and order searching by the given key.
	 * Assign the order of the key in this tree to *order only if order is not NULL.
	 */
	nldb_rc_t get (const void * key, void ** value, nldb_order_t * key_order ) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );


		leaf_node_t * leaf_node = NULL;

		if (key_order == NULL) {
			// Because key_order is NULL, we don't need to get the order of the key. Simply pass NULL as the last argument.
			nldb_rc_t rc = find_leaf_node(key, & leaf_node, NULL);
			if (rc) return rc;

			tx_assert(leaf_node);

			rc = leaf_node->get(key, value, NULL);
			if (rc) return rc;
		} else {
			nldb_order_t key_count_before_leaf_node;
			nldb_rc_t rc = find_leaf_node(key, & leaf_node, & key_count_before_leaf_node);
			if (rc) return rc;

			nldb_order_t key_order_in_the_leaf_node;
			rc = leaf_node->get(key, value, & key_order_in_the_leaf_node);
			if (rc) return rc;

			*key_order = key_count_before_leaf_node + key_order_in_the_leaf_node;
		}


		return NLDB_OK;
	}

	/*! Get the value and order searching by the given key.
	 * Assign the order of the key in this tree to *order only if order is not NULL.
	 */
	nldb_rc_t get (const nldb_order_t key_order, void ** key, void ** value) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );

		nldb_order_t key_count_before_leaf_node;

		leaf_node_t * leaf_node = NULL;

		// The key_order is out of range.
		if (key_order < 1 || key_order > root_node_->key_count() ) {
			return NLDB_ERROR_KEY_NOT_FOUND;
		}

		nldb_rc_t rc = find_leaf_node(key_order, & leaf_node, &key_count_before_leaf_node);
		if (rc) return rc;

		tx_assert(leaf_node);

		// Because the key_order th key exists in the found leaf_node, the keys before the leaf node should be less than key_order.
		tx_debug_assert( key_order > key_count_before_leaf_node );

		// The key order in the leaf node.
		nldb_order_t key_order_in_leaf_node = key_order - key_count_before_leaf_node;

		tx_debug_assert( key_order_in_leaf_node <= leaf_node->key_count() );

		rc = leaf_node->get(key_order_in_leaf_node, key, value);
		if (rc) return rc;

		return NLDB_OK;
	}

	nldb_rc_t del (const void * key, void ** deleted_value)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( deleted_value != NULL );

		leaf_node_t * leaf_node = NULL;

		nldb_rc_t rc = find_leaf_node(key, & leaf_node, NULL /*key_count_before_the_found_node*/);
		if (rc) return rc;

		tx_assert(leaf_node);

		rc = del_from_leaf_node(leaf_node, key, deleted_value);
		if (rc) return rc;


		rc = propagate_key_count_decrement(leaf_node);
		if (rc) return rc;

		// No need to remove the key and the leaf node from the parent node if there is no more key in the node.
		// Because the empty leaf node will be reused when a new key within the range of the empty leaf node is put.

		return NLDB_OK;
	}

	nldb_rc_t seek_forward(const void * key, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( iter != NULL );

		leaf_node_t * leaf_node = NULL;

		nldb_rc_t rc = find_leaf_node(key, & leaf_node, NULL /*key_count_before_the_found_node*/);
		if (rc) return rc;

		// Even when the key does not exist, find_leaf_node returns a node where the key "SHOULD" be inserted.
		// leaf_node should not be NULL in any case.
		tx_assert( leaf_node );

		iter->current_node_ = leaf_node;

		nldb_sorted_array<key_space_size> & keys_with_values = leaf_node->keys_with_values();

		rc = keys_with_values.iter_forward(key, &iter->key_iter_);
		if (rc) return rc;

		return NLDB_OK;
	}

	nldb_rc_t seek_backward(const void * key, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( iter != NULL );

		leaf_node_t * leaf_node = NULL;

		nldb_rc_t rc = find_leaf_node(key, & leaf_node, NULL /*key_count_before_the_found_node*/);
		if (rc) return rc;

		// Even when the key does not exist, find_leaf_node returns a node where the key "SHOULD" be inserted.
		// leaf_node should not be NULL in any case.
		tx_assert( leaf_node );

		iter->current_node_ = leaf_node;

		nldb_sorted_array<key_space_size> & keys_with_values = leaf_node->keys_with_values();

		rc = keys_with_values.iter_backward(key, &iter->key_iter_);
		if (rc) return rc;

		return NLDB_OK;
	}

	/*! Seek forward from the key_order th key. key_order starts from 1.
	 */
	nldb_rc_t seek_forward(const nldb_order_t key_order, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_order > 0 );
		tx_debug_assert( iter != NULL );

		nldb_order_t key_count_before_leaf_node;
		leaf_node_t * leaf_node = NULL;

		// The key_order is out of range.
		if (key_order < 1 || key_order > root_node_->key_count() ) {
			return NLDB_ERROR_KEY_NOT_FOUND;
		}

		nldb_rc_t rc = find_leaf_node(key_order, & leaf_node, &key_count_before_leaf_node);
		if (rc) return rc;

		// Even when the key does not exist, find_leaf_node returns a node where the key "SHOULD" be inserted.
		// leaf_node should not be NULL in any case.
		tx_assert( leaf_node );

		// Because the key_order th key exists in the found leaf_node, the keys before the leaf node should be less than key_order.
		tx_debug_assert( key_order > key_count_before_leaf_node );

		iter->current_node_ = leaf_node;

		nldb_sorted_array<key_space_size> & keys_with_values = leaf_node->keys_with_values();

		// The key order in the leaf node.
		nldb_order_t key_order_in_leaf_node = key_order - key_count_before_leaf_node;

		tx_debug_assert( key_order_in_leaf_node <= keys_with_values.key_count() );

		rc = keys_with_values.iter_forward(key_order_in_leaf_node, &iter->key_iter_);
		if (rc) return rc;

		return NLDB_OK;
	}

	/*! Seek backward from the key_order th key. key_order starts from 1.
	 */
	nldb_rc_t seek_backward(const nldb_order_t key_order, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_order > 0 );
		tx_debug_assert( iter != NULL );

		nldb_order_t key_count_before_leaf_node;
		leaf_node_t * leaf_node = NULL;

		// The key_order is out of range.
		if (key_order < 1 || key_order > root_node_->key_count() ) {
			return NLDB_ERROR_KEY_NOT_FOUND;
		}

		nldb_rc_t rc = find_leaf_node(key_order, & leaf_node, &key_count_before_leaf_node);
		if (rc) return rc;

		// Even when the key does not exist, find_leaf_node returns a node where the key "SHOULD" be inserted.
		// leaf_node should not be NULL in any case.
		tx_assert( leaf_node );

		// Because the key_order th key exists in the found leaf_node, the keys before the leaf node should be less than key_order.
		tx_debug_assert( key_order > key_count_before_leaf_node );

		iter->current_node_ = leaf_node;

		nldb_sorted_array<key_space_size> & keys_with_values = leaf_node->keys_with_values();

		// The key order in the leaf node.
		nldb_order_t key_order_in_leaf_node = key_order - key_count_before_leaf_node;

		tx_debug_assert( key_order_in_leaf_node <= keys_with_values.key_count() );

		rc = keys_with_values.iter_backward(key_order_in_leaf_node, &iter->key_iter_);
		if (rc) return rc;

		return NLDB_OK;
	}

	nldb_rc_t move_forward(iterator_t & iter, void ** key, void ** value, bool * end_of_iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );
		tx_debug_assert( end_of_iter != NULL );

		if ( iter.current_node_ == NULL )
		{
			return NLDB_ERROR_END_OF_ITERATION;
		}

		nldb_sorted_array<key_space_size> & keys_with_values = iter.current_node_->keys_with_values();

		nldb_rc_t rc = keys_with_values.iter_next(iter.key_iter_, key, value);
		if (rc) return rc;

		if (*key == NULL) { // No more keys to iterate

			// Move to next leaf node.
			iter.current_node_ = iter.current_node_->next_;

			if ( iter.current_node_ )
			{
				rc = iter.current_node_->keys_with_values().iter_forward(&iter.key_iter_);
				if (rc) return rc;

				rc = iter.current_node_->keys_with_values().iter_next(iter.key_iter_, key, value);
				if (rc) return rc;

				if (*key == NULL) {
					tx_debug_assert( *value == NULL );
					// No more nodes to iterate.
					return NLDB_ERROR_END_OF_ITERATION;
				}
			}
			else
			{
				// No more nodes to iterate.
				return NLDB_ERROR_END_OF_ITERATION;
			}
		}

		return NLDB_OK;
	}

	nldb_rc_t move_backward(iterator_t & iter, void ** key, void ** value, bool * end_of_iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );
		tx_debug_assert( end_of_iter != NULL );

		if ( iter.current_node_ == NULL )
		{
			return NLDB_ERROR_END_OF_ITERATION;
		}

		nldb_sorted_array<key_space_size> & keys_with_values = iter.current_node_->keys_with_values();

		nldb_rc_t rc = keys_with_values.iter_prev(iter.key_iter_, key, value);
		if (rc) return rc;

		if (*key == NULL) { // No more keys to iterate

			// Move to previous leaf node.
			iter.current_node_ = iter.current_node_->prev_;

			if ( iter.current_node_ )
			{
				rc = iter.current_node_->keys_with_values().iter_backward(&iter.key_iter_);
				if (rc) return rc;

				rc = iter.current_node_->keys_with_values().iter_prev(iter.key_iter_, key, value);
				if (rc) return rc;

				if (*key == NULL) {
					tx_debug_assert( *value == NULL );
					// No more nodes to iterate.
					return NLDB_ERROR_END_OF_ITERATION;
				}
			}
			else
			{
				// No more nodes to iterate.
				return NLDB_ERROR_END_OF_ITERATION;
			}
		}

		return NLDB_OK;
	}

	/*! Return the total nubmer of keys in the tree.
	 */
	nldb_rc_t get_key_count(nldb_order_t * total_key_count ) const
	{
		tx_debug_assert(total_key_count);

		tx_debug_assert(root_node_);

		// The root node has the total number of keys in the whole tree.
		*total_key_count = root_node_->key_count();

		return NLDB_OK;
	}

};

#endif /* NLDB_ARRAY_TREE_H_ */

