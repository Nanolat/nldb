/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Kangmo Kim
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

int main(int argc, char** argv)  
{  
    testing::InitGoogleTest(&argc, argv);  
    RUN_ALL_TESTS();  
	return 0;
} 


