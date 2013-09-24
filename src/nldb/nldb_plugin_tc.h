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

#ifndef _NLDB_PLUGIN_TC_H_
#define _NLDB_PLUGIN_TC_H_ (1)

#include <nldb/nldb_plugin.h>

class nldb_plugin_tc_t : public nldb_plugin_t
{
public:
	virtual ~nldb_plugin_tc_t() {};

	virtual nldb_rc_t table_create(const nldb_table_id_t db_id, const nldb_table_id_t table_id, nldb_plugin_table_desc_t * table_desc);

	virtual nldb_rc_t table_drop(nldb_plugin_table_desc_t & table_desc);

	virtual nldb_rc_t table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx );

	virtual nldb_rc_t table_close(nldb_table_context_t table_ctx );

	virtual nldb_rc_t table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value);

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order);

// errors : NLDB_ORDER_OUT_OF_RANGE
	virtual nldb_rc_t table_get(nldb_table_context_t table_ctx, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value );

// errors : NLDB_ERROR_KEY_NOT_FOUND
	virtual nldb_rc_t table_del(nldb_table_context_t table_ctx, const nldb_key_t & key);

	virtual nldb_rc_t table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat);

	// Free allocated memory for the value if any.
	virtual nldb_rc_t value_free(nldb_value_t & value);

	// Free allocated memory for the key if any.
	virtual nldb_rc_t key_free(nldb_key_t & key);

	virtual nldb_rc_t cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx);

	virtual nldb_rc_t cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_key_t & key);

	// errors : NLDB_ERROR_CURSOR_NOT_OPEN
	virtual nldb_rc_t cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_order_t & order);

	// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
	virtual nldb_rc_t cursor_fetch(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value, nldb_order_t * order);

	virtual nldb_rc_t cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx);
};

#endif // _NLDB_PLUGIN_TC_H_
