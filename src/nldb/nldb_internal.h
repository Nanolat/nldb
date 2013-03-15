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

#ifndef _O_NLDB_INTERNAL_H_
#define _O_NLDB_INTERNAL_H_ (1)

// All internal symbols are defined in this header. 
// This header is not exposed as part of the library interface header files.

// The initial transaction ID which is assigned to the first transaction that calls begin function.
#define INITIAL_TRANSACTION_ID (1)

// For linux platform, we need to define _snprintf, which exists only in windows platform.
#define _snprintf snprintf

// How long should we sleep for master and slaves to handshake each other?
#define XS_HANDSHAKE_SLEEP_SECONDS (10)

// for ntohs, ntohl
#include <arpa/inet.h>
// for be64toh
#include <endian.h>

// The keys in tree are encoded in big endian to simply use memcmp for key comparison.
// To get the key from three nodes, we need to convert the key from big endian to host machine encoding.
inline unsigned long long get_uint64_key(const void * key, int key_length)
{
	unsigned long long k;
	switch(key_length)
	{
	case 1:
		k = (unsigned long long) *((unsigned char*)key);
		break;
	case 2:
		k = (unsigned long long) ntohs( *((unsigned short*)key) ) ;
		break;
	case 4:
		k = (unsigned long long) ntohl( *((unsigned int*)key) ) ;
		break;
	case 8:
		k = be64toh( *((unsigned long long*)key) );
		break;
	default:
		k = 0;
		break;
	}
	return k;
}

#endif


