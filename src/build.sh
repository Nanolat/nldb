#########################################################################
echo "Step 1. Setting environment variables ..."
#########################################################################
# Nanolat Database library file, libnldb.so will be located in src/nldb .
# Before building Nanolat Database, setup LD_LIBRARY_PATH to include external libraries built in step 3 and src/nldb where libnldb.so is located.
#########################################################################


if [ `echo $LD_LIBRARY_PATH | grep "nldb" | wc -l` = "1" ]
then
    echo "Environment variables are already set."
else
    source env.sh
fi

#########################################################################
echo "Step 2. Building nldb, tests, and gtest ..."
#########################################################################

for build_dir in nldb tests gtest
do
    pushd .
    cd $build_dir/build
    make clean
    make
    popd
done

