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

#ifndef _TREE_TEST_COMMON_H
#define _TREE_TEST_COMMON_H (1)

//////////////////////////////////////////////////////////////////////////////
// Common header file for tests of nldb_array_tree and nldb_sorted_array.
//////////////////////////////////////////////////////////////////////////////

// We need to test private and protected members as well.
// Compile private and protected members as public members.
#define private public
#define protected public

#include "nldb_array_tree.h"
#include "nldb_sorted_array.h"

#define TREE_KEY_LEN (4)
#define TREE_KEYS_PER_NODE (3)
#define TREE_KEY_SPACE_SIZE (TREE_KEY_LEN * TREE_KEYS_PER_NODE)

typedef nldb_array_tree<TREE_KEY_SPACE_SIZE> array_tree_t;
typedef array_tree_t::node_t tree_node_t;
typedef array_tree_t::internal_node_t tree_internal_node_t;
typedef array_tree_t::leaf_node_t tree_leaf_node_t;


// Macros for simple test data with five keys and values
#define KEY_01                "0001"
#define VALUE_01              "value-0001"
#define KEY_01_LEN            (strlen(KEY_01))
#define VALUE_01_LEN          (strlen(VALUE_01))
extern  tree_internal_node_t* NODE_01;

#define KEY_02                "0002"
#define VALUE_02              "value-0002"
#define KEY_02_LEN            (strlen(KEY_02))
#define VALUE_02_LEN          (strlen(VALUE_02))
extern  tree_internal_node_t* NODE_02;

#define KEY_03                "0003"
#define VALUE_03              "value-0003"
#define KEY_03_LEN            (strlen(KEY_03))
#define VALUE_03_LEN          (strlen(VALUE_03))
extern  tree_internal_node_t* NODE_03;

#define KEY_04                "0004"
#define VALUE_04              "value-0004"
#define KEY_04_LEN            (strlen(KEY_04))
#define VALUE_04_LEN          (strlen(VALUE_04))
extern  tree_internal_node_t* NODE_04;

#define KEY_05                "0005"
#define VALUE_05              "value-0005"
#define KEY_05_LEN            (strlen(KEY_05))
#define VALUE_05_LEN          (strlen(VALUE_05))
extern  tree_internal_node_t* NODE_05;

#define KEY_01_MATCHES(x)     ((x)!=NULL && (memcmp((x), KEY_01, KEY_01_LEN)==0))
#define KEY_02_MATCHES(x)     ((x)!=NULL && (memcmp((x), KEY_02, KEY_02_LEN)==0))
#define KEY_03_MATCHES(x)     ((x)!=NULL && (memcmp((x), KEY_03, KEY_03_LEN)==0))
#define KEY_04_MATCHES(x)     ((x)!=NULL && (memcmp((x), KEY_04, KEY_04_LEN)==0))
#define KEY_05_MATCHES(x)     ((x)!=NULL && (memcmp((x), KEY_05, KEY_05_LEN)==0))

#define VALUE_01_MATCHES(x)   ((x)!=NULL && (memcmp((x), VALUE_01, VALUE_01_LEN)==0))
#define VALUE_02_MATCHES(x)   ((x)!=NULL && (memcmp((x), VALUE_02, VALUE_02_LEN)==0))
#define VALUE_03_MATCHES(x)   ((x)!=NULL && (memcmp((x), VALUE_03, VALUE_03_LEN)==0))
#define VALUE_04_MATCHES(x)   ((x)!=NULL && (memcmp((x), VALUE_04, VALUE_04_LEN)==0))
#define VALUE_05_MATCHES(x)   ((x)!=NULL && (memcmp((x), VALUE_05, VALUE_05_LEN)==0))

#define NODE_01_MATCHES(x)    ((x)==NODE_01)
#define NODE_02_MATCHES(x)    ((x)==NODE_02)
#define NODE_03_MATCHES(x)    ((x)==NODE_03)
#define NODE_04_MATCHES(x)    ((x)==NODE_04)
#define NODE_05_MATCHES(x)    ((x)==NODE_05)


#define KEY_MATCHES(x, key)    ((x)!=NULL && (memcmp((x), key, strlen(key))==0))
#define VALUE_MATCHES(x, data) KEY_MATCHES(x, data)
#define NODE_MATCHES(x, node)  (x==node)

#endif // _TREE_TEST_COMMON_H
