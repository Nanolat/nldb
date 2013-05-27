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
#ifdef __APPLE__
# include <libkern/OSByteOrder.h>

# define htobe16(x) OSSwapHostToBigInt16(x)
# define htole16(x) OSSwapHostToLittleInt16(x)
# define be16toh(x) OSSwapBigToHostInt16(x)
# define le16toh(x) OSSwapLittleToHostInt16(x)
 
# define htobe32(x) OSSwapHostToBigInt32(x)
# define htole32(x) OSSwapHostToLittleInt32(x)
# define be32toh(x) OSSwapBigToHostInt32(x)
# define le32toh(x) OSSwapLittleToHostInt32(x)
 
# define htobe64(x) OSSwapHostToBigInt64(x)
# define htole64(x) OSSwapHostToLittleInt64(x)
# define be64toh(x) OSSwapBigToHostInt64(x)
# define le64toh(x) OSSwapLittleToHostInt64(x)
#else
// for be64toh
# include <endian.h>
#endif

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


