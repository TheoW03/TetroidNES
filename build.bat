@echo off
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.7.2\mingw_64\  -DCMAKE_CXX_FLAGS="-std=c++17"  -S . -B build
cmake --build build --config Release
cd build
TetroidNES.exe