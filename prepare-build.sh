#########################################################################
echo "Step 1. Installing prerequisites..."
#########################################################################
# Install bzip2, zlib, and unzip. In CentOS, build.sh runs following commands.
#     sudo yum install zlib-devel bzip2-devel unzip
# In Ubuntu, build.sh runs following commands.
#    sudo apt-get install libz-dev libbz2-dev unzip
#########################################################################

if [ `lsb_release -a | grep CentOS | wc -l` -gt 0 ]
then
   USE_YUM=1 
fi

if [ `lsb_release -a | grep "Red Hat Enterprise Linux" | wc -l` -gt 0 ]
then
   USE_YUM=1
fi

if [ `lsb_release -a | grep "Ubuntu" | wc -l` -gt 0 ]
then 
   USE_APT=1
fi

if [ "$USE_YUM" = "1" ]
then
   echo "Using yum to install prerequisites."
   sudo yum install zlib-devel bzip2-devel unzip
fi

if [ "$USE_APT" = "1" ]
then
   echo "Using aptitude to install prerequisites."
   sudo apt-get install libz-dev libbz2-dev unzip
fi


#########################################################################
echo "Step 2. Check gcc version ... (needs 4.6.x) ..."
#########################################################################
# Nanolat Database requires gcc 4.6 to build it.
#    gcc --version
#
# If the version of gcc is not 4.6.x, build gcc 4.6 by running following commands.
#    git clone https://github.com/ThankyouSoft/gcc46.git
#    cd gcc46
#    ./build.sh
#
# Use the gcc 4.6 built in the previous step. 
# This is required for each time you compile nldb project.
#    cd gcc46
#    source ./use-gcc46.sh
#########################################################################

if [ `gcc --version | grep "4\.6\." | wc -l` = "1" ]
then
   echo "gcc version is 4.6.x. Will use the installed gcc."
elif [ `gcc --version | grep "4\.7\." | wc -l` = "1" ]
then
   echo "gcc version is 4.7.x. Will use the installed gcc."
else
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
# Nanolat Database requires following libraries.
# 1. Google test framework for unit tests.
# 2. Crossroads library for sending and receiving transactional logs between the master and slaves.
# 3. TokyoCabinet library for the TokyoCabinet in-memory table plugin.
# 4. Google LevelDB library for the LevelDB on-disk table plugin.
#########################################################################

cd external-libs
./build.sh
cd ..


#########################################################################
echo "Step 4. Build all projects ..."
#########################################################################
make


