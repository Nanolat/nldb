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

#ifndef NLDB_DEBUGGABLE_ARRAY_TREE_H_
#define NLDB_DEBUGGABLE_ARRAY_TREE_H_

#include "nldb_array_tree.h"
/*
 * debuggable_array_tree : an array tree that can print keys in nodes to console and check consistency.
 */

template<int key_space_size> class nldb_debuggable_array_tree : public nldb_array_tree<key_space_size> {
protected :
	// The super type definition. To access fields or functions in nldb_array_tree<key_space_size>, we will use the prefix super:: instead of nldb_array_tree<key_space_size>:: .
	#define SUPER typename nldb_array_tree<key_space_size>

	// The previous key printed in print_node.
	// This is to check consistency by verifying that the keys printed in print_node are growing.
	// Because print_node prints keys by running depth first search, the keys visited should be ordered.
	mutable void * prev_printed_key;

#define INDENT_PER_DEPTH (2)

	nldb_rc_t check_prev_key_less_than_or_equal_to_new_key(void* prev_key, void*new_key) const {
		int cmp = this->compare_keys( prev_key, new_key);
		if ( cmp > 0 )
		{
			std::string prev_key_str = get_string_key(prev_key, this->key_length_);
			std::string new_key_str = get_string_key(new_key, this->key_length_);
			printf("invalid key order : prev_key : %s, cur_key : %s\n", prev_key_str.c_str(), new_key_str.c_str());
			return NLDB_ERROR;
		}
		return NLDB_OK;
	}

	nldb_rc_t print_leaf_node(int indent, SUPER::leaf_node_t * node ) const
	{
		typename nldb_sorted_array<key_space_size>::iterator_t iter;

		nldb_rc_t rc = node->keys_with_values().iter_forward(&iter);
		if (rc) return rc;

		printf("%*s[ LEAF-NODE (%llX) (keys:%lld) (parent:%llX)(p:%llX)(n:%llX) ]\n", indent, " ", (uint64_t)node, (uint64_t)node->key_count(), (uint64_t)node->parent(), (uint64_t)node->prev_, (uint64_t)node->next_);
		while(1)
		{
			void * key = NULL;
			void * value = NULL;
			rc = node->keys_with_values().iter_next(iter, (void**)&key, (void**)&value);
			if (rc) return rc;

			if (key == NULL)
				break;

			tx_debug_assert(value != NULL);

			std::string key_str = get_string_key(key, this->key_length_);
			printf("%*sKEY:%s\n", indent, " ", key_str.c_str() );

			if (prev_printed_key) {
				tx_assert( check_prev_key_less_than_or_equal_to_new_key(prev_printed_key, key) == NLDB_OK );
			}
			prev_printed_key = key;
		}

		return NLDB_OK;
	}

	nldb_rc_t print_node(int indent, SUPER::node_t * node ) const
	{
		if ( node->is_internal_node() )
		{
			nldb_rc_t rc = print_internal_node( indent + INDENT_PER_DEPTH, (SUPER::internal_node_t*) node );
			if (rc) return rc;
		}
		else if (node->is_leaf_node() )
		{
			nldb_rc_t rc = print_leaf_node( indent + INDENT_PER_DEPTH, (SUPER::leaf_node_t*) node );
			if (rc) return rc;
		}

		return NLDB_OK;
	}

	nldb_rc_t print_internal_node(int indent, SUPER::internal_node_t * node ) const
	{
		typename nldb_sorted_array<key_space_size>::iterator_t iter;

		nldb_rc_t rc = node->keys_with_right_children().iter_forward(&iter);
		if (rc) return rc;

		printf("%*s[ INTERNAL-NODE (%llX) (keys:%lld) (parent:%llX) ]\n", indent, " ", (uint64_t)node, (uint64_t)node->key_count(), (uint64_t)node->parent());

		printf("%*sLEFT CHILD:\n", indent, " " );

		SUPER::node_t * left_child = node->left_child();
		tx_debug_assert(left_child);

		print_node(indent + INDENT_PER_DEPTH, left_child);

		while(1)
		{
			void * key = NULL;
			SUPER::node_t * child_node = NULL;
			rc = node->keys_with_right_children().iter_next(iter, (void**)&key, (void**)&child_node);
			if (rc) return rc;

			if (key == NULL)
				break;

			tx_debug_assert(child_node != NULL);

			std::string str_key = get_string_key(key, this->key_length_);

			printf("%*sKEY:%s\n", indent, " ", str_key.c_str() );

			if (prev_printed_key) {
				tx_assert( check_prev_key_less_than_or_equal_to_new_key(prev_printed_key, key) == NLDB_OK );
			}
			prev_printed_key = key;

			print_node(indent + INDENT_PER_DEPTH, child_node);
		}

		return NLDB_OK;
	}

public :

	int get_print_iteration() {
		static int printed_times = 0;
		return printed_times++;
	}

	// Make sure that all keys in the leaf node are sorted in order.
	void check_consistency(int iteration) const {
		unsigned long long min = 0LL;
		SUPER::iterator_t iter;

		printf("checking tree consistency[%d]\n",iteration);

		nldb_rc_t rc = this->seek_forward(&min, &iter);
		tx_assert(rc==0);

		void * prev_key = NULL;

		while (1) {
			void * key = NULL;
			void * value = NULL;
			bool end_of_iter = false;
			rc = this->move_forward(iter, &key, &value, &end_of_iter);
			if ( rc == NLDB_ERROR_END_OF_ITERATION)
				break;

			tx_assert(rc==0);

			if (end_of_iter)
				break;

			if (prev_key != NULL)
			{
				// Make sure that the previous key is less than the current key.
				int cmp = this->compare_keys( prev_key, key);
				if ( cmp >=0 )
				{
					printf("assertion fail : should be  %llu < %llu\n", get_uint64_key(prev_key, this->key_length_), get_uint64_key(key, this->key_length_) );
					tx_assert(0);
				}
			}
			prev_key = key;
		}

	}


	void print_tree(int iteration) const
	{
		prev_printed_key = NULL;

		printf("ARRAY TREE[%d]=========================\n", iteration);

		this->template print_internal_node( 0, this->root_node_ );
	}

#define PRINT_DEBUG_TRACE (1)

	nldb_rc_t put (const void * key, const void * value)
	{
		tx_debug_assert( this->is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( value != NULL );

#if defined(PRINT_DEBUG_TRACE)
		int i = get_print_iteration();
#endif

		nldb_rc_t rc = nldb_array_tree<key_space_size>::put(key, value);
		if (rc) return rc;

#if defined(PRINT_DEBUG_TRACE)
		printf("<<after put : %s>>\n", get_string_key(key, this->key_length_).c_str() );
		print_tree(i);
		check_consistency(i);
#endif
		return NLDB_OK;
	}

	nldb_rc_t del (const void * key, void ** deleted_value)
	{
		tx_debug_assert( this->is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( deleted_value != NULL );

#if defined(PRINT_DEBUG_TRACE)
		int i = get_print_iteration();

		printf("<<before del : %s>>\n", get_string_key(key, this->key_length_).c_str());
		print_tree(i);
		check_consistency(i);
#endif
		nldb_rc_t rc = nldb_array_tree<key_space_size>::del(key, deleted_value);
		if (rc) return rc;

		return NLDB_OK;
	}

};
#endif /* NLDB_DEBUGGABLE_ARRAY_TREE_H_ */
