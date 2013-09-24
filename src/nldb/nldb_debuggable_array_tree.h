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

		printf("%*s[ LEAF-NODE (%llX) (keys:%lld) (parent:%llX)(p:%llX)(n:%llX) ]\n", indent, " ",
				(long long unsigned int)node,
				(long long unsigned int)node->key_count(),
				(long long unsigned int)node->parent(),
				(long long unsigned int)node->prev_,
				(long long unsigned int)node->next_);
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

		printf("%*s[ INTERNAL-NODE (%llX) (keys:%lld) (parent:%llX) ]\n", indent, " ",
				(long long unsigned int)node,
				(long long unsigned int)node->key_count(),
				(long long unsigned int)node->parent());

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

//#define PRINT_DEBUG_TRACE (1)

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
