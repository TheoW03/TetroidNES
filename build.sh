!#/bin/bash/
cmake --trace-expand -DCMAKE_PREFIX_PATH=~/Qt/6.7.2/gcc_64/ -DCMAKE_CXX_FLAGS="-std=c++17"  -S . -B build
cmake  --build build --config Release