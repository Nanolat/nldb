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

#ifndef NLDB_OBJECT_POOL_H_
#define NLDB_OBJECT_POOL_H_ (1)

#include <assert.h>
#include <stdlib.h>

class nldb_object_pool {
public :
	nldb_object_pool() {
	}

	void * malloc(int size) {
		void * p = ::malloc(size);
		assert(p);
		return p;
	}
	void free( void * p ) {
		assert(p);
		::free(p);
	}
private :
	size_t object_size_;
};

#endif /* NLDB_OBJECT_POOL_H_ */
