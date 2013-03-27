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
- Simply run build.sh which runs following steps. Currently the script was tested on CentOS 6.3. 
    - Install required libraries.
    - Check gcc version and install gcc 4.6.3 if gcc version is not 4.6.x.
    - Build external libraries in external-libs.
    - Setup environment variable LD_LIBRARY_PATH.
    - Build src/nldb/libnldb.so, src/tests/tests, src/gtest/gtest.

- After the script finishes, you have following binaries.

    - src/nldb/libnldb.so : The shared library file of Nanolat Database
    - src/tests/tests : The testing program to run master/slave process of Nanolat Database.
    - src/gtest/gtest : The test program implements unit tests with Google Test Framework.

- Optionally, you can use eclipse for your development environment, but you need to run build.sh at least once to prepare prerequisites and dependent libraries.
    - run eclipse, switch your workspace to nldb/src .
    - Import all projects under nldb/src .
    - clean nldb, gtest, tests projects. 
    - build nldb, gtest, tests projects. 

How to run a test for the master-slave replication example?
-----------------------------------------------------------
Before running a test, build it by running build.sh .
Before runnigg a test, set LD_LIBRARY_PATH by executing *env.sh* command after going to the src directory.

    cd src
    source env.sh

Create a directory for a slave database, and start a slave process of the *tests* program.

    cd src
    source env.sh
    mkdir slave
    cd slave
    cp ../tests .
    ./tests nldb-slave

Start a master process.

    cd src
    source env.sh
    ./tests nldb-master

How to compile and link my program?
-----------------------------------
Add following directories as additional include paths.

    src/include
    src/txbase

Specify std=c++0x to compile your program using c++0x.

For example, in case you use g++, use following command line to compile your program where ${NLDB_HOME} has this README.md file.

    g++ -I${NLDB_HOME}/src/include -I${NLDB_HOME}/src/txbase -c -std=c++0x -o "your_program.o" "your_program.cpp"

Add following directories as additional library paths.

    external-libs/lib
    src/nldb

To link your program, add following libraries.

    nldb
    leveldb
    tokyocabinet
    xs

For example, in case you use g++, use following command line to link your program where ${NLDB_HOME} has this README.md file.

    g++ -L{NLDB_HOME}/external-libs/lib -L{NLDB_HOME}/nldb -o your_program_binary  your_program.o -lnldb -lleveldb -ltokyocabinet -lxs

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
