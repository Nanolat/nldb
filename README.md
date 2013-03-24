Nanolat Database
================

What is it?
-----------
- An in-memory key/value store supporting put/get/del, and cursors for range search.
- Supports transaction and master-slave replication.
- Supports Table Plugins to implement your own data structure for efficient data processing for your specific domain.

Target systems
--------------
- Financial systems such as HFT(High Frequency Trading), FEP(Front End Processor), etc.
- Monitoring systems that require processing huge amount of data in a second.

Why use it?
-----------
- Low latency data processing, 1 transaction takes < 5 us.
- Supports 1.5M transactions / second .
- Supports built in Table Plug-ins using google LevelDB and TokyoCabinet providing replication feature on top of them. 
- (Under Development in another project) Supports basic SQL that is directly converted to Nanolat Database API at compile time to achieve low latency data processing.

How it works?
-------------
- Uses 40G Infiniband and 10G Ethernet instead of TCP/IP to replicate data from a server to another server within 2.5 us.
- Instead of flushing transactional log on local disk, transfer it to slave machine.

How to build?
-------------
- Simply run build.sh which runs following steps. Currently following platforms are tested. 
1. CentOS 6.3

- Step 1. Install prerequisites. 
Install bzip2, zlib, and unzip. In CentOS, build.sh runs following commands.
    sudo yum install zlib-devel bzip2-devel unzip
In Ubuntu, build.sh runs following commands.
    sudo apt-get install libz-dev libbz2-dev unzip

- Step 2. Check gcc version and download and build gcc 4.6 if not exists.
Nanolat Database requires gcc 4.6 to build it. 
    gcc --version

If the version of gcc is not 4.6.x, build gcc 4.6 by running following commands.
    git clone https://github.com/ThankyouSoft/gcc46.git
    cd gcc46
    ./bld.sh

To use the gcc 4.6 built in the previous step. This is required for each time you compile nldb project. 
    cd gcc46
    source ./use-gcc46.sh

- Step 3. Build external libraries.
Nanolat Database requires following libraries.
1. Google test framework for unit tests.
2. Crossroads library for sending and receiving transactional logs between the master and slaves.
3. TokyoCabinet library for the TokyoCabinet in-memory table plugin.
4. Google LevelDB library for the LevelDB on-disk table plugin.

- Step 4. Set environment variables.
Nanolat Database library file, libnldb.so will be located in src/nldb . 
Before building Nanolat Database, setup LD_LIBRARY_PATH to include external libraries built in step 3 and src/nldb where libnldb.so is located.

- Step 5. Build nldb, tests, and gtest 
The last step builds following binary files.
1. src/nldb/libnldb.so : The shared library file of Nanolat Database
2. src/tests/tests : The testing program to run master/slave process of Nanolat Database.
3. src/gtest/gtest : The test program implements unit tests with Google Test Framework.

- Optionally, you can use eclipse for your development environment. 
1. run eclipse, switch your workspace to nldb/src .
2. Import all projects under nldb/src .
3. clean nldb, gtest, tests projects. 
4. build nldb, gtest, tests projects. 

Nanolat Open Source License v1.0 ( Simple but ambiguous )
---------------------------------------------------------
- If you are in USA, Canada, or Germany, it is *likely* that you get free LGPL v2.1.  
- In other countries, you are *likely* under GPL v3, but you can buy LGPL v2.1 from Nanolat Co., Ltd. 
- For free software, you *definitely* get free LGPL v2.1 whereever you are.

Nanolat Open Source License v1.0 ( Detailed and explicit )
----------------------------------------------------------
- In the hope of seeing increasing open source contributions in countries that do not have enough open source contributors, Nanolat Open Source License provides free LGPL v2.1 only to companies and people of countries that have at least three KudoRank 10 contributors in http://www.ohloh.net/people. The list of countries are updated by the beginning of a year. In 2013, these countries are USA, Canada, and Germany.
- For companies and people of other countries, you are under GPL v3.0, but you can buy LGPL v2.1 license for a product from Nanolat Co., Ltd.
- However, for non-commercial usage, you get free LGPL v2.1 whereever you are.
- A company is of a country if the head of office is in the country.
- A person is of a country if the nationality of the person is the country.
- A country has a KudoRank 10 contributor if the contributor's location is the country in http://www.ohloh.net/.
