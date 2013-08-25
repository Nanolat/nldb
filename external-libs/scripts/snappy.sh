EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/snappy-1.1.0.tar.gz
cd snappy-1.1.0
./configure
make clean
make
make install DESTDIR=`pwd`/dest
cp dest/usr/local/include/* ${EXT_LIB_HOME}/include
cp dest/usr/local/lib/* ${EXT_LIB_HOME}/lib

