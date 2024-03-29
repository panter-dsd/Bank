cd /var/tmp
mkdir programming
cd programming
mkdir Bank
cd Bank

mkdir build_unix_release
cd build_unix_release
cmake -D CMAKE_BUILD_TYPE=Release /mnt/work/program/Bank
cd ..

mkdir build_unix_debug
cd build_unix_debug
cmake  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug /mnt/work/program/Bank
cd ..

