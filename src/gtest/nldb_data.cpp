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
 
#include <string.h>

#include "nldb_data.h"

const long exepceted_value1 = 10001;
const long exepceted_value2 = 10002;
const long exepceted_value3 = 10003;
const long exepceted_value4 = 10004;
const long exepceted_value5 = 10005;
const long exepceted_value6 = 10006;
const long exepceted_value7 = 10007;
const long exepceted_value8 = 10007;

const nldb_order_t exepcetedOrder_order1 = 1;
const nldb_order_t exepcetedOrder_order2 = 2;
const nldb_order_t exepcetedOrder_order3 = 3;
const nldb_order_t exepcetedOrder_order4 = 4;
const nldb_order_t exepcetedOrder_order5 = 5;
const nldb_order_t exepcetedOrder_order6 = 6;
const nldb_order_t exepcetedOrder_order7 = 7;
const nldb_order_t exepcetedOrder_order8 = 8;


DECLARE_KEY_VALUE(r_bmin,    10,    0, -1, -1); // for searching purpose
DECLARE_KEY_VALUE(r1,        50,    0, exepceted_value1, exepcetedOrder_order1);
DECLARE_KEY_VALUE(r2,       100, 1000, exepceted_value2, exepcetedOrder_order2);
DECLARE_KEY_VALUE(r3,       200,  200, exepceted_value3, exepcetedOrder_order3);
DECLARE_KEY_VALUE(r_3_4,    200,  250, -1, -1); // for searching purpose
DECLARE_KEY_VALUE(r4,       200,  300, exepceted_value4, exepcetedOrder_order4);
DECLARE_KEY_VALUE(r5,      1000,  100, exepceted_value5, exepcetedOrder_order5);
DECLARE_KEY_VALUE(r_5_6,   1000,  150, -1, -1); // for searching purpose
DECLARE_KEY_VALUE(r6,      1000,  200, exepceted_value6, exepcetedOrder_order6);
DECLARE_KEY_VALUE(r7,      2000,  100, exepceted_value7, exepcetedOrder_order7);
DECLARE_KEY_VALUE(r8,      2000,  200, exepceted_value8, exepcetedOrder_order5);
DECLARE_KEY_VALUE(r_amax, 10000,    0, -1, -1); // for searching purpose
