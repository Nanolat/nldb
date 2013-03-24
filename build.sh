#########################################################################
echo "Step 1. Installing prerequisites..."
#########################################################################
if [ `lsb_release -a | grep CentOS | wc -l` = "1" ]
then
   USE_YUM=1 
fi

if [ `lsb_release -a | grep "Red Hat Enterprise Linux" | wc -l` = "1" ]
then
   USE_YUM=1
fi

if [ `lsb_release -a | grep "Ubuntu" | wc -l` = "1" ]
then 
   USE_APT=1
fi

if [ $USE_YUM -eq 1 ]
then
   sudo yum install zlib-devel bzip2-devel unzip
fi

if [ $USE_APT -eq 1 ]
then
   sudo apt-get install libz-dev libbz2-dev unzip
fi


#########################################################################
echo "Step 2. Check gcc version ... (needs 4.6.x) ..."
#########################################################################

if [ `gcc --version | grep 4.6 | wc -l` = "0" ]
then
   echo "gcc version is not 4.6.x. Will download and install gcc 4.6.3."
   git clone https://github.com/ThankyouSoft/gcc46.git
   cd gcc46
   ./build.sh
   source use-gcc46.sh
   cd ..
fi

#########################################################################
echo "Step 3. Build external libraries ..."
#########################################################################

cd external-libs
./build.sh
cd ..


#########################################################################
echo "Step 4. Setting environment variables ..."
#########################################################################

cd src
source env.sh

#########################################################################
echo "Step 5. Building nldb, tests, and gtest ..."
#########################################################################

for build_dir in nldb test gtest
do
    pushd .
    cd $build_dir/build
    make clean
    make
    popd 
done


