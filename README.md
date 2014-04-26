Nanolat Database
================

What is it?
-----------
- An in-memory key/value store supporting put/get/del, and cursors for range search.
- Supports searching keys by rank. (Ex> Get 100 keys from the 1000th key)
- Supports transaction and master-slave replication.
- Adds transactional log replication feature on top of LevelDB and TokyoCabinet.
- Supports Table Plugins to implement your own data structure for efficient data processing for your specific domain.

Target systems
--------------
- Leaderboard servers sorting users by scores, getting the rank of a user. 
- Financial systems such as HFT(High Frequency Trading), FEP(Front End Processor), etc.
- Monitoring systems that require processing huge amount of data in a second.

Why use it?
-----------
- Low latency data processing, 1 transaction takes < 5 us.
- Supports 1.5M transactions / second .
- Supports built in Table Plug-ins using LevelDB and TokyoCabinet providing replication feature on top of them. 
- (Under Development in another project) Supports basic SQL that is directly converted to Nanolat Database API at compile time to achieve low latency data processing.

How it works?
-------------
- Uses 40G Infiniband and 10G Ethernet instead of TCP/IP to replicate data from a server to another server within 2.5 us.
- Instead of flushing transactional log on local disk, transfer it to slave machine.

How to build?
-------------
- In MacOS, you need to install gcc 4.7 using MacPorts.
    - Download MacPorts from http://www.macports.org/install.php
    - Install gcc 4.7 using MacPorts by running following two commands.

        sudo port install gcc47
        
        sudo port select --set gcc mp-gcc47

    - gcc 4.6 in MacOS is not supported yet.

- Simply run 'make prepare-build' to prepare compiler and build required libraries. Currently the script was tested on CentOS 6.3, Ubuntu 12.10, and MacOS 10.7
    - Install required libraries.
    - Check gcc version and install gcc 4.6.3 if gcc version is neither 4.6 nor 4.7.
    - Build external libraries in external-libs.
    - Setup environment variable LD_LIBRARY_PATH(Or DYLD_LIBRARY_PATH in OSX).
 
- And then, run 'make'. It will build following binaries.    
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

    g++ -I${NLDB_HOME}/src/include -I${NLDB_HOME}/src/txbase -c -std=c++0x -o your_program.o your_program.cpp

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

# Apache v2 License

http://www.apache.org/licenses/LICENSE-2.0.html
