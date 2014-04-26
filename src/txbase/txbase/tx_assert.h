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

#ifndef _O_TX_ASSERT_H_ 
#define _O_TX_ASSERT_H_ (1)

#include <assert.h>

#define tx_assert assert

# ifdef DEBUG
#   define tx_debug_assert(expr) tx_assert(expr)
# else
#   define tx_debug_assert(expr)
# endif 

#endif //_O_TX_ASSERT_H_

