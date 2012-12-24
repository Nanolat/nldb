source ./env.sh
for test in gtest tests
do
    for configuration in GccDebug GccRelease IntelDebug IntelRelease
    do
        echo "Running $test with $configuration."
        pushd .
            cd $test/$configuration
            if [ $test = "tests" ] 
            then
                ./$test nldb-master
            else
                ./$test 
            fi
        popd
    done 
done
