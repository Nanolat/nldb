profmerge -prof_dir ../nldb/IndelDebug -prof_dir ../tests/IntelDebug

pushd .
cp nldb-comp.txt ../nldb/IntelDebug
cd ../nldb/IntelDebug
profmerge
codecov -comp nldb-comp.txt
firefox CODE_COVERAGE.HTML
popd 

