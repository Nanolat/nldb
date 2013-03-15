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

#ifndef NLDBSEARCH_H_
#define NLDBSEARCH_H_

#include <nldb/nldb_common.h>

#include <txbase/tx_assert.h>

#include "nldb_internal.h"

#include <stdio.h>

// memmove
#include <string.h>

const int NLDB_SA_MIN_KEY_SIZE=1;

/*
 * nldb_sorted_array : a sorted array of fixed size keys. Each key maps to a void* data.
 */
template <int key_space_size> class nldb_sorted_array {
private :
	char   key_space_[key_space_size];
	void * data_[key_space_size/NLDB_SA_MIN_KEY_SIZE];
	int    max_key_count_;
	int    key_count_;
	int    used_key_space_;
	key_length_t key_length_;

	inline int compare_keys(const char* key1, const char* key2) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key1 != NULL );
		tx_debug_assert( key2 != NULL );

		return memcmp(key1, key2, key_length_);
	}


	inline void next_key(int & key_pos, int & data_pos) const
	{
		tx_debug_assert( is_initialized() );

		key_pos  += key_length_;
		data_pos ++;
	}

	inline void prev_key(int & key_pos, int & data_pos) const
	{
		tx_debug_assert( is_initialized() );

		key_pos  -= key_length_;
		data_pos --;
	}

	// Search forward. For the keys stored in key_space_, the given key will be positioned as follows.
	// Assume : Key size is 2 byte (big endian encoded) integer, and key_space_ has three keys, 3,5,and 7.
	// This picture shows the *out_key_pos and *out_data_pos value for the given input keys from 0.
	//
	// given                   +-------+-------+-------+
	// key_space_   =>         |   3   |   5   |   7   |
	//                         +-------+-------+-------+
	//
	// with             +------+-------+-------+-------+
	// key =>           | 0,1,2|  3,4  |  5,6  |7,8,...|
	//                  +------+-------+-------+-------+
    //                      |      |       |       |
	//                      V      V       V       V
	// maps             +------+-------+-------+-------+
	// *out_key_pos =>  |  -1  |   0   |   2   |   4   |
	//                  +------+-------+-------+-------+
	// maps             +------+-------+-------+-------+
	// *out_data_pos => |  -1  |   0   |   1   |   2   |
	//                  +------+-------+-------+-------+
	//
	// This function is used in following use cases.
	// Let's say key_pos is *out_key_pos returned by this function.
	// Let's say data_pos is *out_data_pos returned by this function.
	//
	// (1) internal node for tree traversal
	//     if (key_pos < 0) traverse to the left child.
	//     otherwise use
	//
	// (2) forward cursor on leaf node. Search all keys keys >= KEY )
	//     before returning the first key check followings.
	//     a. if (key_pos < 0) key_pos ++;
	//     b. if (KEY matches with key_space_[key_pos]) start returning the key from key_pos.
	//        else start returning key from (key_pos + 1)
	//
	// (3) put operation - put a key with mapping a data pointer.
    //     b. if (*out_key_found) replace the key/data at the key_pos and data_pos.
	//        else insert the new key at key_space_[key_pos+key_length_], insert the new data at data_[data_pos+1]
	//
	// (4) get operation - return data pointer that is mapped with a given key.
	//     a. if (key_pos < 0) return "KEY_NOT_FOUND"
	//     a. if (*out_key_found) return data_[data_pos]
	//        else return NULL.
	//
	// (5) del operation
	//     a. if (*out_data_found) remove the key at key_space_[key_pos], remove the data at data_[data_pos]
	//
	// TODO : Optimize using binary search.
	nldb_rc_t search_forward(const void * key, int * out_key_pos, int * out_data_pos, bool * out_key_found) const
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( key != NULL );
		tx_debug_assert( out_key_pos != NULL );
		tx_debug_assert( out_data_pos != NULL );
		tx_debug_assert( out_key_found != NULL );

		int prev_cmp = -1; // The result of compare_keys for the previous loop

		int key_pos = -key_length_; // key_pos
		int data_pos = -1; // data_pos

		bool key_found = false;
		for (const char * k = key_space_;
			 k < key_space_ + used_key_space_; ) {

			int cmp = compare_keys( (const char*)key, k );
			if ( cmp < 0 )
			{
				break;
			}
			prev_cmp =cmp;

			key_pos  += key_length_;
			data_pos ++;

			k += key_length_;
		}

		// The key comparison of the previous loop : The key matched. This means the key was found in the key_space_
		if ( prev_cmp == 0)
			key_found = true;

		*out_key_pos = key_pos;
		*out_data_pos = data_pos;
		*out_key_found = key_found;

		return NLDB_OK;
	}

	// Search backward. For the keys stored in key_space_, the given key will be positioned as follows.
	// Assume : Key size is 2 byte (big endian encoded) integer, and key_space_ has three keys, 3,5,and 7.
	// This picture shows the *out_key_pos and *out_data_pos value for the given input keys from 0.
	//
	// given            +-------+-------+-------+
	// key_space_   =>  |   3   |   5   |   7   |
	//                  +-------+-------+-------+
	//
	// with             +-------+-------+-------+-------+
	// key =>           |0,1,2,3|  4,5  |  6,7  |8,9,...|
	//                  +-------+-------+-------+-------+
    //                      |       |       |       |
	//                      V       V       V       V
	// maps             +-------+-------+-------+-------+
	// *out_key_pos =>  |   0   |   2   |   4   |   6   |
	//                  +-------+-------+-------+-------+
	// maps             +-------+-------+-------+-------+
	// *out_data_pos => |   0   |   1   |   2   |   3   |
	//                  +-------+-------+-------+-------+
	//
	// This function is used in following use cases.
	// Let's say key_pos is *out_key_pos returned by this function.
	// Let's say data_pos is *out_data_pos returned by this function.
	//
	// (1) backward cursor on leaf node. Search all keys keys <= KEY )
	//     before returning the first key check followings.
	//     a. if (key_pos > used_key_space_ ) key_pos --;
	//     b. if (KEY matches with key_space_[key_pos]) start returning the key from key_pos.
	//        else start returning key from (key_pos - 1)
	//
	nldb_rc_t search_backward(const void * key, int * out_key_pos, int * out_data_pos, bool * out_key_found) const
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( key != NULL );
		tx_debug_assert( out_key_pos != NULL );
		tx_debug_assert( out_data_pos != NULL );
		tx_debug_assert( out_key_found != NULL );

		int prev_cmp = -1; // The result of compare_keys for the previous loop

		int key_pos = used_key_space_; // key_pos
		int data_pos = key_count_; // data_pos
		bool key_found = false;
		for (const char * k = (key_space_ + used_key_space_ - key_length_);
			 k >= key_space_ ; ) {

			int cmp = compare_keys( k, (const char*)key  );
			if ( cmp < 0 )
			{
				break;
			}

			prev_cmp = cmp;

			key_pos  -= key_length_;
			data_pos --;

			k -= key_length_;
		}

		// The key comparison of the previous loop : The key matched. This means the key was found in the key_space_
		if ( prev_cmp == 0)
			key_found = true;

		*out_key_pos = key_pos;
		*out_data_pos = data_pos;
		*out_key_found = key_found;

		return NLDB_OK;
	}

	void set_data(int data_pos, const void * data)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( data_pos < max_key_count_ );
		tx_debug_assert( data != NULL );

		data_[data_pos] = (void*) data;
	}

	void * get_data(int data_pos) const
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( data_pos < max_key_count_ );

		return data_[data_pos];
	}

	void * get_key(int key_pos) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= 0 );
		tx_debug_assert( key_pos + key_length_ <= key_space_size );

		return (void*)(key_space_ + key_pos);
	}

	// move keys right by the key length from the given key position.
	inline void move_keys_right_by_the_key_length_from(int key_pos)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= 0 );
		tx_debug_assert( key_pos + key_length_ < key_space_size );

		char * shift_src = key_space_+key_pos;
		char * shift_dest = key_space_+key_pos+key_length_;
		int size_of_keys_to_shift = used_key_space_ - key_pos;

		tx_debug_assert( size_of_keys_to_shift > 0 );

		// Make sure that both the keys to shift are within the key_space_.
		tx_debug_assert( shift_src >= key_space_);
		tx_debug_assert( shift_dest > key_space_);
		tx_debug_assert( shift_src + size_of_keys_to_shift < key_space_ + key_space_size);
		tx_debug_assert( shift_dest + size_of_keys_to_shift <= key_space_ + key_space_size);

		// The dest and src overlaps, so use memmove instead of memcpy.
		memmove(shift_dest, shift_src, size_of_keys_to_shift);
	}

	// move keys left by the key length from the given key position.
	// The key right before the given key_pos is removed.
	inline void move_keys_left_by_the_key_length_from(int key_pos)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= key_length_ );
		tx_debug_assert( key_pos < key_space_size );

		char * shift_src = key_space_+key_pos;
		char * shift_dest = key_space_+key_pos-key_length_;

		int size_of_keys_to_shift = used_key_space_ - key_pos;

		tx_debug_assert( size_of_keys_to_shift > 0 );

		// Make sure that both the keys to shift are within the key_space_.
		tx_debug_assert( shift_src > key_space_);
		tx_debug_assert( shift_dest >= key_space_);
		tx_debug_assert( shift_src + size_of_keys_to_shift <= key_space_ + key_space_size);
		tx_debug_assert( shift_dest + size_of_keys_to_shift < key_space_ + key_space_size);

		// The dest and src overlaps, so use memmove instead of memcpy.
		memmove(shift_dest, shift_src, size_of_keys_to_shift);
	}

	inline void set_key_data_at(int key_pos, int data_pos, const void * key, const void * data)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= 0 );
		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( key_pos + key_length_ <= key_space_size );
		tx_debug_assert( data_pos < max_key_count_ );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );

		// copy key
		memcpy(key_space_ + key_pos, key, key_length_);

		// set value
		(void)set_data(data_pos, data);
	}

	inline void move_data_right_by_one_from(int data_pos)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( data_pos + 1 < max_key_count_ );

		char * shift_src = (char*) &data_[data_pos];
		char * shift_dest = (char*) &data_[data_pos + 1];
		int size_of_data_to_shift = (char*)&data_[key_count_] - (char*)&data_[data_pos];

		tx_debug_assert( size_of_data_to_shift > 0 );

		// Make sure that both the keys to shift are within the key_space_.
		tx_debug_assert( shift_src >= (char*)data_);
		tx_debug_assert( shift_dest > (char*)data_);
		tx_debug_assert( shift_src + size_of_data_to_shift <  (char*)&data_[max_key_count_]);
		tx_debug_assert( shift_dest + size_of_data_to_shift <= (char*)&data_[max_key_count_]);

		// The dest and src overlaps, so use memmove instead of memcpy.
		memmove(shift_dest, shift_src, size_of_data_to_shift);
	}

	inline void move_data_left_by_one_from(int data_pos)
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( data_pos > 0 );
		tx_debug_assert( data_pos < max_key_count_ );

		char * shift_src = (char*)&data_[data_pos];
		char * shift_dest = (char*)&data_[data_pos - 1];
		int size_of_data_to_shift = (char*)&data_[key_count_] - (char*)&data_[data_pos];

		tx_debug_assert( size_of_data_to_shift > 0 );

		// Make sure that both the keys to shift are within the key_space_.
		tx_debug_assert( shift_src > (char*)data_);
		tx_debug_assert( shift_dest >= (char*)data_);
		tx_debug_assert( shift_src + size_of_data_to_shift <= (char*)&data_[max_key_count_]);
		tx_debug_assert( shift_dest + size_of_data_to_shift < (char*)&data_[max_key_count_]);

		// The dest and src overlaps, so use memmove instead of memcpy.
		memmove(shift_dest, shift_src, size_of_data_to_shift);
	}


	inline void insert_key_data_at(int key_pos, int data_pos, const void * key, const void * data)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= 0 );
		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( key_pos + key_length_ <= key_space_size );
		tx_debug_assert( data_pos + 1 <= max_key_count_ );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );

		if (key_pos == used_key_space_) { // Append at the end of the key_space_
			// No keys to to move
			tx_debug_assert( data_pos == key_count_);
		}
		else
		{
			tx_debug_assert( data_pos < key_count_);
			(void)move_keys_right_by_the_key_length_from(key_pos);
			(void)move_data_right_by_one_from(data_pos);
		}

		(void)set_key_data_at(key_pos, data_pos, key, data);

		key_count_++;
		used_key_space_ += key_length_;
	}

	inline void remove_key_data_at(int key_pos, int data_pos)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( key_pos >= 0 );
		tx_debug_assert( data_pos >= 0 );
		tx_debug_assert( key_pos + key_length_ <= key_space_size );
		tx_debug_assert( data_pos < max_key_count_ );

		if ( key_pos + key_length_ == used_key_space_ )
		{
			// No keys to move
			tx_debug_assert( data_pos + 1 == key_count_ );
		}
		else
		{
			tx_debug_assert( data_pos + 1 < key_count_ );

			// We have keys and data to move left
			(void)move_keys_left_by_the_key_length_from(key_pos + key_length_ );
			(void)move_data_left_by_one_from(data_pos + 1);
		}

		key_count_ --;
		used_key_space_ -= key_length_;
	}

public:
	typedef struct iterator_t {
		int key_pos_;
		int data_pos_;
	} iterator_t;

	nldb_sorted_array()
	{
		max_key_count_ = 0;
		key_count_ = 0;
		used_key_space_ = 0;
		key_length_ = 0;
	}

	virtual ~nldb_sorted_array()
	{
	}

	nldb_rc_t init(const key_length_t & key_length)
	{
		// allow init to be called multiple times.
		// tx_debug_assert( ! is_initialized() );

		tx_debug_assert( key_length > 0 );

		max_key_count_ = sizeof(key_space_)/key_length;
		key_count_ = 0;
		used_key_space_ = 0;
		key_length_ = key_length;

		return NLDB_OK;
	}


	nldb_rc_t init(const key_length_t & key_length, int key_count, int used_key_space, char * key_space, void **data_area)
	{
		// allow init to be called multiple times.
		// tx_debug_assert( ! is_initialized() );

		tx_debug_assert( key_length > 0 );
		tx_debug_assert( key_count > 0 );
		tx_debug_assert( used_key_space > 0);
		tx_debug_assert( key_space != NULL);
		tx_debug_assert( data_area != NULL);

		tx_debug_assert( used_key_space / key_length == key_count );


		max_key_count_ = sizeof(key_space_)/key_length;

		key_count_ = key_count;
		used_key_space_ = used_key_space;
		key_length_ = key_length;

		// copy key and data.
		memcpy( key_space_, key_space, used_key_space);
		memcpy( data_, data_area, sizeof(void*) * key_count);

		return NLDB_OK;
	}

	nldb_rc_t destroy()
	{
		tx_debug_assert( is_initialized() );

		return NLDB_OK;
	}

	nldb_rc_t put(const void * key, const void * data)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );
		tx_debug_assert( key_length_ > 0 );

		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_forward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if ( key_found ) { // The exact key is found. Replace the value.
			(void)set_data(data_pos, data);
		} else { // The exact key is not found. Insert the key at the position.
			(void)insert_key_data_at(key_pos + key_length_, data_pos+1, key, data);
		}
		return NLDB_OK;
	}

	nldb_rc_t get(const void * key, void ** data ) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );


		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_forward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if (key_found)
		{
			// The key was found.
			*data = get_data(data_pos);
		}
		else
		{
			// We don't have the key.
			*data = NULL;
		}

		return NLDB_OK;
	}

	// Set the data of the first less than or equal key in key_space_ to *data.
	// Set NULL to *data in case the key_space_ does not have any key greater than or equal to the given key.
	//
	// c.f. "le" means "less than or equal to".
	nldb_rc_t find_first_le_key(const void * key, void ** data ) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );

		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_forward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if (data_pos < 0) // The first key in the key_space_ is greater than the given key.
		{
			*data = NULL; // We don't have the key greater than or equal to the given key. Set NULL.
		}
		else
		{
			*data = get_data(data_pos);
		}

		return NLDB_OK;
	}

	nldb_rc_t del(const void * key, void ** data )
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );

		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_forward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if (key_found)
		{
			// The key was found.
			*data = get_data(data_pos);
			(void) remove_key_data_at( key_pos, data_pos );
		}
		else
		{
			// We don't have the key.
			*data = NULL;
		}

		return NLDB_OK;
	}

	nldb_rc_t remove_max_key( void ** key, void ** data )
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );
		tx_debug_assert( key_length_ > 0 );

		if ( key_count_ > 0 )
		{
			tx_debug_assert( used_key_space_ >= key_length_ );

			int max_key_pos = used_key_space_ - key_length_;
			int max_data_pos = key_count_ - 1;

			*key = get_key( max_key_pos );
			*data = get_data( max_data_pos );

			(void) remove_key_data_at( max_key_pos, max_data_pos);
		}

		return NLDB_OK;
	}

	nldb_rc_t iter_forward(const void * key, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( iter != NULL );
		tx_debug_assert( key_length_ > 0 );

		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_forward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if (!key_found) // The key does not match. Move one key forward to locate the first key to iterate.
		{
			key_pos += key_length_;
			data_pos ++;
		}

		iter->key_pos_ = key_pos;
		iter->data_pos_ = data_pos;

		return NLDB_OK;
	}

	nldb_rc_t iter_backward(const void * key, iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( iter != NULL );
		tx_debug_assert( key_length_ > 0 );

		int key_pos = 0;
		int data_pos = 0;
		bool key_found = false;
		nldb_rc_t rc = search_backward(key, &key_pos, &data_pos, &key_found);
		if (rc) return rc;

		if (!key_found) // The key does not match. Move one key backward to locate the first key to iterate.
		{
			key_pos -= key_length_;
			data_pos --;
		}

		iter->key_pos_ = key_pos;
		iter->data_pos_ = data_pos;

		return NLDB_OK;
	}

	// Iterate forward for all keys within the sorted array.
	nldb_rc_t iter_forward(iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( iter != NULL );
		tx_debug_assert( key_length_ > 0 );

		iter->key_pos_ = 0;
		iter->data_pos_ = 0;

		return NLDB_OK;
	}

	// Iterate backward for all keys within the sorted array.
	nldb_rc_t iter_backward(iterator_t * iter) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( iter != NULL );
		tx_debug_assert( key_length_ > 0 );

		iter->key_pos_ = used_key_space_-key_length_;
		iter->data_pos_ = key_count_-1;

		return NLDB_OK;
	}

	nldb_rc_t iter_next(iterator_t & iter, void ** key, void ** data) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( iter.key_pos_ >= 0 );
		tx_debug_assert( iter.data_pos_ >= 0 );

		if ( iter.key_pos_ < used_key_space_ )
		{
			tx_debug_assert( iter.data_pos_ < key_count_ );

			*key = (void*) (key_space_ + iter.key_pos_);
			*data = data_[iter.data_pos_];

			(void)next_key(iter.key_pos_, iter.data_pos_);
		}
		else
		{
			// No more key in this sorted array.
			*key = NULL;
			*data = NULL;
		}

		return NLDB_OK;
	}

	nldb_rc_t iter_prev(iterator_t & iter, void ** key, void ** data) const
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( key != NULL );
		tx_debug_assert( data != NULL );
		tx_debug_assert( key_length_ > 0 );

		tx_debug_assert( iter.key_pos_ + key_length_ <= used_key_space_ );
		tx_debug_assert( iter.data_pos_ < key_count_ );

		if ( iter.key_pos_ >= 0 )
		{
			tx_debug_assert( iter.data_pos_ >= 0 );

			*key = (void*) (key_space_ + iter.key_pos_);
			*data = data_[iter.data_pos_];

			(void)prev_key(iter.key_pos_, iter.data_pos_);
		}
		else
		{
			// No more key in this sorted array.
			*key = NULL;
			*data = NULL;
		}

		return NLDB_OK;
	}

	nldb_rc_t merge_with(nldb_sorted_array<key_space_size> * right)
	{
		tx_debug_assert( is_initialized() );
		tx_debug_assert( right != NULL );

		// Not implemented yet. Merging nodes is P2. We will implement the feature after the first release of NLDB.
		return NLDB_ERROR_UNSUPPORTED_FEATURE;
	}

	nldb_rc_t split(nldb_sorted_array<key_space_size> * right_half )
	{
		tx_debug_assert( is_initialized() );

		tx_debug_assert( right_half != NULL );
		tx_debug_assert( right_half->key_count() == 0 );

		tx_debug_assert( key_length_ > 0 );

		// The split sorted array should have at least two keys.
		tx_debug_assert( key_count_ >= 2);

		// Calculate the new key count on the new right node.
		int right_half_key_count = key_count_ >> 1; // key_count_ / 2
		int right_half_used_key_space = right_half_key_count * key_length_ ;

		// Calculate the new key count on this node.
		int new_key_count = key_count_ - right_half_key_count;
		int new_used_key_space = used_key_space_ - right_half_used_key_space;

		// Calculate the address of key_space_ and data_ from where the right_half copies key and data.
		char * right_half_key_space = key_space_ + new_used_key_space;
		void ** right_half_data_area = & data_[new_key_count];


		// Copy the key and data.
		nldb_rc_t rc = right_half->init(key_length_,
				                        right_half_key_count,
				                        right_half_used_key_space,
				                        right_half_key_space,
				                        right_half_data_area);
		if (rc) return rc;

		key_count_ = new_key_count;
		used_key_space_ = new_used_key_space;

		return NLDB_OK;
	}

	bool is_initialized() const {
		return (key_length_ > 0) ? true : false;
	}

	int key_count() const {
		tx_debug_assert( is_initialized() );

		return key_count_;
	}

	int key_length() const {
		tx_debug_assert( is_initialized() );

		return key_length_;
	}

	int used_key_space() {
		tx_debug_assert( is_initialized() );

		return used_key_space_;
	}

	bool is_empty() const
	{
		tx_debug_assert( is_initialized() );

		return (used_key_space_ == 0 ) ? true : false;
	}

	bool is_full() const
	{
		tx_debug_assert( is_initialized() );

		return (used_key_space_ < key_space_size) ? false : true;
	}

	// Return NULL if no key exists in this node.
	// Otherwise return the minimum key in this node.
	const void * min_key() const
	{
		tx_debug_assert( is_initialized() );

		if (used_key_space_ == 0)
		{
			return NULL;
		}

		return key_space_;
	}
};

#endif /* NLDBSEARCH_H_ */
