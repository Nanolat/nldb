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


#ifndef _NLDB_COMMON_H_
#define _NLDB_COMMON_H_ (1)

#define NLDB_OK                         (0)
#define NLDB_ERROR_CODE_START           (-1)
#define NLDB_ERROR                      (-1)
#define NLDB_ERROR_INVALID_ARGUMENT     (-2)
#define NLDB_ERROR_DB_ALREADY_EXISTS    (-3)
#define NLDB_ERROR_DB_NOT_FOUND         (-4)
#define NLDB_ERROR_TABLE_ALREADY_EXISTS (-5)
#define NLDB_ERROR_TABLE_NOT_FOUND      (-6)
#define NLDB_ERROR_TX_ALREADY_BEGAN     (-7)
#define NLDB_ERROR_TX_NOT_BEGAN         (-8)
#define NLDB_ERROR_TX_NOT_ENDED         (-9)
#define NLDB_ERROR_KEY_NOT_FOUND        (-10)
#define NLDB_ERROR_CURSOR_NOT_OPEN      (-11)
#define NLDB_ERROR_END_OF_ITERATION     (-12)
#define NLDB_ERROR_INVALID_ITERATION     (-13)
#define NLDB_ERROR_TX_ALREADY_INITIALIZED (-14)
#define NLDB_ERROR_TX_ALREADY_DESTROYED (-15)
#define NLDB_ERROR_PLUGIN_NO_MORE_SLOT  (-16)
#define NLDB_ERROR_NO_MEM               (-17)
#define NLDB_ERROR_NOT_IMPLEMENTED_YET  (-18)
#define NLDB_ERROR_INIT_REPLICATION_MESSAGE (-19)
#define NLDB_ERROR_UNSUPPORTED_FEATURE  (-20)
#define NLDB_ERROR_VARIABLE_KEY_SIZE_NOT_SUPPORTED  (-21)
#define NLDB_ERROR_VARIABLE_VALUE_SIZE_NOT_SUPPORTED  (-22)
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
	NLDB_CURSOR_INVALID_DIRECTION = 0,
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

