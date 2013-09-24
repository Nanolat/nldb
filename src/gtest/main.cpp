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


