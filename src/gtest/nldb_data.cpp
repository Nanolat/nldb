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
