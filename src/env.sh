OS_NAME=`uname`
ADD_LIBS=`pwd`/../external-libs/lib:`pwd`/nldb

if [ $OS_NAME = "Darwin" ] 
then
   echo "OSX Detected."
   export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$ADD_LIBS
   echo "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH"
else
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ADD_LIBS
   echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
fi
