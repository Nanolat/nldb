EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/tokyocabinet-1.4.48.tar.gz
cd tokyocabinet-1.4.48
./configure
make clean
make
cp *.h ${EXT_LIB_HOME}/include
cp libtokyocabinet* ${EXT_LIB_HOME}/lib

