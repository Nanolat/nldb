EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/libxs-1.2.0.tar.gz
cd libxs-1.2.0
./configure
make clean
make
cp -r include/* ${EXT_LIB_HOME}/include
cp src/.libs/libxs.* ${EXT_LIB_HOME}/lib

