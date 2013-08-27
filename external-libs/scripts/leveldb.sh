EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/leveldb-1.10.0.tar.gz
cd leveldb-1.10.0
# no configure file exists for leveldb
make clean
make
cp -r include/* ${EXT_LIB_HOME}/include
cp libleveldb* ${EXT_LIB_HOME}/lib

