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

#include <string>
#include <sstream>

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
	case 16:
		k = be64toh( *((unsigned long long*)key) );
		break;
	default:
		k = 0;
		break;
	}
	return k;
}

inline std::string get_string_key(const void * key, int key_length)
{
	unsigned long long int_key = get_uint64_key(key, key_length);

	if (int_key != 0) {
		std::stringstream stream;

		stream << int_key;

		return stream.str();
	}

	return std::string((const char*)key, key_length);
}

#endif


