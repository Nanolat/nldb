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


#include "nldb_plugin_tc.h"

#include <tcutil.h>

nldb_rc_t nldb_plugin_tc_t::table_create(const nldb_table_id_t db_id, const nldb_table_id_t table_id, nldb_plugin_table_desc_t * table_desc)
{
	// no table descriptor to store.
	table_desc->length = 0;
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_tc_t::table_drop(nldb_plugin_table_desc_t & table_desc)
{
	// nothing to remove from disk
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_tc_t::table_open(nldb_plugin_table_desc_t & table_desc, nldb_table_context_t * table_ctx)
{
	TCNDB * ndb = tcndbnew();
	*table_ctx = ndb;
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_tc_t::table_close(nldb_table_context_t table_ctx)
{
	TCNDB * ndb = (TCNDB*) table_ctx;
	tcndbdel(ndb);

	return NLDB_OK;
}

// errors : NLDB_ERROR_KEY_ALREADY_EXISTS
nldb_rc_t nldb_plugin_tc_t::table_put(nldb_table_context_t table_ctx, const nldb_key_t & key, const nldb_value_t & value)
{
	TCNDB * ndb = (TCNDB*) table_ctx;

	tcndbput(ndb, key.data, key.length, value.data, value.length);

	return NLDB_OK;
}


// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_tc_t::table_get(nldb_table_context_t table_ctx, const nldb_key_t & key, nldb_value_t * value, nldb_order_t * order)
{
	TCNDB * ndb = (TCNDB*) table_ctx;

	if (order != NULL)
		return NLDB_ERROR_UNSUPPORTED_FEATURE;

	int value_size;
	void * value_ptr = tcndbget(ndb, key.data, key.length, &value_size);
	if (!value_ptr) // the value is not found
		return NLDB_ERROR_KEY_NOT_FOUND;

	value->data = value_ptr;
	value->length = value_size;

	return NLDB_OK;
}

// errors : NLDB_ERROR_ORDER_OUT_OF_RANGE
nldb_rc_t nldb_plugin_tc_t::table_get(nldb_table_context_t table_ctx, const nldb_order_t & order, nldb_key_t * key, nldb_value_t * value)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

// errors : NLDB_ERROR_KEY_NOT_FOUND
nldb_rc_t nldb_plugin_tc_t::table_del(nldb_table_context_t table_ctx, const nldb_key_t & key)
{
	TCNDB * ndb = (TCNDB*) table_ctx;

	bool deleted = tcndbout(ndb, key.data, key.length);

	if (! deleted)
		return NLDB_ERROR_KEY_NOT_FOUND;

	return NLDB_OK;
}

nldb_rc_t nldb_plugin_tc_t::table_stat(nldb_table_context_t table_ctx, nldb_table_stat_t * table_stat)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}


nldb_rc_t nldb_plugin_tc_t::value_free(nldb_value_t & value) {
	tcfree(value.data);
	value.length = 0;
	return NLDB_OK;
}

nldb_rc_t nldb_plugin_tc_t::key_free(nldb_key_t & value) {
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

nldb_rc_t nldb_plugin_tc_t::cursor_open(nldb_table_context_t table_ctx, nldb_cursor_context_t * cursor_ctx)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}


nldb_rc_t nldb_plugin_tc_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_key_t & key)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN
nldb_rc_t nldb_plugin_tc_t::cursor_seek(nldb_cursor_context_t cursor_ctx, const nldb_cursor_direction_t & direction, const nldb_order_t & order)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

// errors : NLDB_ERROR_CURSOR_NOT_OPEN, NLDB_ERROR_END_OF_ITERATION
nldb_rc_t nldb_plugin_tc_t::cursor_fetch(nldb_cursor_context_t cursor_ctx, nldb_key_t * key, nldb_value_t * value, nldb_order_t * order)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}

nldb_rc_t nldb_plugin_tc_t::cursor_close(nldb_table_context_t table_ctx, nldb_cursor_context_t cursor_ctx)
{
	return NLDB_ERROR_UNSUPPORTED_FEATURE;
}
