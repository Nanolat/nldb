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

 
#ifndef NLDB_DATA_H_
#define NLDB_DATA_H_

#include <gtest/gtest.h>

// for htonl
#include <arpa/inet.h>

#include <nldb/nldb_common.h>
#include <nldb/nldb.h>

#include <stdint.h>
using namespace std;

typedef struct tbl_key {
	uint32_t key1;
	uint32_t key2;
} tbl_key ;
typedef struct tbl_value{
	long value;
} tbl1_value ;

// We can't call htonl within the list of initializing values of global variables.
// However, macro function can be used, so we define htonl_macro here.
#define switch_endian32(v) (unsigned int)( ((((unsigned int)v)&0x000000FFU) << 24) | \
                                           ((((unsigned int)v)&0x0000FF00U) << 8)  | \
                                           ((((unsigned int)v)&0x00FF0000U) >> 8)    | \
                                           ((((unsigned int)v)&0xFF000000U) >> 24) )


#define DEFINE_KEY_VALUE(var)         \
extern tbl_key      var##_key;        \
extern tbl_value    var##_value;      \
extern nldb_key_t   var##_nldb_key;   \
extern nldb_value_t var##_nldb_value; \
extern nldb_order_t var##_nldb_order;

// About using htonl : Keys are stored in big endian encoding in NLDB.
// This is to quickly compare keys by calling memcmp on keys whenever NLDB searches a key.
#define DECLARE_KEY_VALUE(var, k1, k2, v, o)                                   \
tbl_key      var##_key        = { switch_endian32(k1), switch_endian32(k2) };  \
tbl_value    var##_value      = { v};                                          \
nldb_key_t   var##_nldb_key   = { & var##_key, sizeof(var##_key) };            \
nldb_value_t var##_nldb_value = { & var##_value, sizeof(var##_value) };        \
nldb_order_t var##_nldb_order = o;

#define KEY(var)    var##_nldb_key
#define VALUE(var)  var##_nldb_value

#define IS_KEY_EQUAL(var, actualKey) ( (var##_nldb_key.length == actualKey.length) && (memcmp(var##_nldb_key.data, actualKey.data, actualKey.length) == 0) )
#define IS_VALUE_EQUAL(var, actualValue) ( (var##_nldb_value.length == actualValue.length) && (memcmp(var##_nldb_value.data, actualValue.data, actualValue.length) == 0) )
#define IS_ORDER_EQUAL(var, actualOrder) ( var##_nldb_order == (actualOrder) )

/* Data used in nldb_cursor_test.cpp and nldb_order_test.cpp */
DEFINE_KEY_VALUE(r_bmin); // for searching purpose
DEFINE_KEY_VALUE(r1);
DEFINE_KEY_VALUE(r2);
DEFINE_KEY_VALUE(r3);
DEFINE_KEY_VALUE(r_3_4); // for searching purpose
DEFINE_KEY_VALUE(r4);
DEFINE_KEY_VALUE(r5);
DEFINE_KEY_VALUE(r_5_6); // for searching purpose
DEFINE_KEY_VALUE(r6);
DEFINE_KEY_VALUE(r7);
DEFINE_KEY_VALUE(r8);
DEFINE_KEY_VALUE(r_amax); // for searching purpose


#endif /* NLDB_TEST_H_ */
