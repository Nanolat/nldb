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

#include <iostream>

#include "gtest/gtest.h"

/*
TEST(sample_test_case, sample_test) 
{ 
    EXPECT_EQ(1, 1); 
} 

void Foo(float value) {
	printf("value=>%f",value);
}


TEST(sample_test_case, tbb) 
{ 
	float v[] = {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
    // BUGBUG write some test code
} 
*/

/*
 * List of tests to add.
 * 1) Get by Order.
 *    (LevelDB, TC should fail, ArrayTree should succeed)
 * 2) Get by Order. Open Cursor with a Key, move forward until it reaches to the end.
 *    (LevelDB, TC should fail, ArrayTree should succeed)
 * 3) Get by Key with Order Statistics.
 *    (LevelDB, TC should fail, ArrayTree should succeed)
 * 4) Get table statistics
 *    (LevelDB, TC should fail, ArrayTree should succeed)
 * 5) Put the same key, check the count of keys in root node does not change.
 */

/*
 * List of tests to add.
 * 1) Check db parameter. If invalid or NULL, return NLDB_ERROR_INVALID_ARGUMENT.
 * 2) Check if the transaction already began. Change code to return error code instead of assertion, and add test.
 */

/*
 * List of tests to add.
 * 1) Test put/get/del in case the size of key space in array tree is not multiple of key size. ex> key space size = 64, key size = 20.
 */

/*
 * List of tests to add.
 * 1) Variable length value.
 */



int main(int argc, char** argv)  
{  
    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();
} 


