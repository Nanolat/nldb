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


#ifndef _NLDB_COMMON_H_
#define _NLDB_COMMON_H_ (1)

#define NLDB_OK                         (0)
#define NLDB_ERROR_CODE_START           (-1)
#define NLDB_ERROR                      (-1)
#define NLDB_ERROR_DB_ALREADY_EXISTS    (-2)
#define NLDB_ERROR_DB_NOT_FOUND         (-3)
#define NLDB_ERROR_TABLE_ALREADY_EXISTS (-4)
#define NLDB_ERROR_TABLE_NOT_FOUND      (-5)
#define NLDB_ERROR_TX_ALREADY_BEGAN     (-6)
#define NLDB_ERROR_TX_NOT_BEGAN         (-7)
#define NLDB_ERROR_TX_NOT_ENDED         (-8)
#define NLDB_ERROR_KEY_NOT_FOUND        (-9)
#define NLDB_ERROR_CURSOR_NOT_OPEN      (-10)
#define NLDB_ERROR_END_OF_ITERATION     (-11)
#define NLDB_ERROR_INVALID_ITERATION     (-12)
#define NLDB_ERROR_TX_ALREADY_INITIALIZED (-13)
#define NLDB_ERROR_TX_ALREADY_DESTROYED (-14)
#define NLDB_ERROR_PLUGIN_NO_MORE_SLOT  (-15)
#define NLDB_ERROR_NO_MEM               (-16)
#define NLDB_ERROR_NOT_IMPLEMENTED_YET  (-17)
#define NLDB_ERROR_INIT_REPLICATION_MESSAGE (-18)
#define NLDB_ERROR_UNSUPPORTED_FEATURE  (-19)
#define NLDB_ERROR_VARIABLE_KEY_SIZE_NOT_SUPPORTED  (-20)
#define NLDB_ERROR_VARIABLE_VALUE_SIZE_NOT_SUPPORTED  (-21)
#define NLDB_ERROR_ORDER_OUT_OF_RANGE (-22)
/* Unused, but required to know the maximum number for the error code */
#define NLDB_ERROR_CODE_END            (-22)

typedef int nldb_int32_t;
typedef unsigned int nldb_uint32_t;
typedef unsigned short nldb_uint16_t;
typedef long long nldb_int64_t;
typedef unsigned long long nldb_uint64_t;

typedef int nldb_rc_t;
typedef int nldb_db_id_t;
typedef int nldb_table_id_t;
typedef long long nldb_tx_id_t;
typedef nldb_uint32_t nldb_plugin_id_t;

typedef long long nldb_order_t;
typedef unsigned short key_length_t;
typedef unsigned short value_length_t;

typedef enum nldb_cursor_direction_t {
	NLDB_CURSOR_FORWARD = 1,
	NLDB_CURSOR_BACKWARD = 2
} nldb_cursor_direction_t;

typedef struct nldb_key_t {
	void * data;
	key_length_t length;
} nldb_key_t;

typedef struct nldb_value_t {
	void * data;
	value_length_t length;
} nldb_value_t;

typedef struct nldb_table_stat_t {
	nldb_int64_t key_count;
} nldb_table_stat_t ;

#endif //_NLDB_COMMON_H_

