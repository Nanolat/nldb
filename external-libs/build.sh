# The path where following directories exist.
# archives - contains tarballs or zip files of open source projects on that Nanolat Database depends.
# lib - contains libraries built.
# include - contains header files of libraries.
# scripts - build scripts written in bash.
# log - the output files of each build scripts in scripts folder.


EXT_LIB_HOME=`pwd`

# run sudo once, not to ask the root password in the mid of building dependencies
sudo echo "Build started at `date`"

#Read all arguments into a variable.
DEP_LIST=$@

if [ $# -eq 1 ] 
then
    if [ $1 = "all" ]
    then
        echo "clean up, build all dependencies."
        # List of project on which SoTopless depends.
        DEP_LIST="clean gtest snappy leveldb xs tc"
    fi
fi 

for dep in $DEP_LIST
do
    echo $dep
    if [ $dep = "clean" ]
    then
        rm -rf include lib bin build log
        mkdir include lib bin build log
    else
        pushd .
        cd build 
        echo "##########################################################"
        echo "building $dep"
        echo "##########################################################"
        LOG=../log/${dep}.log
        echo "Started at `date`" 
        ../scripts/${dep}.sh | tee $LOG 
        echo "Finished at `date`"
        popd 
    fi 
done
