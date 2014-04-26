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
