!#/bin/bash/
cmake  -DCMAKE_PREFIX_PATH=~/Qt/6.8.0/gcc_64/ -DCMAKE_CXX_FLAGS="-std=c++17"  -S . -B build
cmake  --build build --config Release
./build/TetroidNES