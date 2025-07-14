@echo off
cmake -G "MinGW Makefiles" -S . -B build
cmake --build build --config Release
cd build
%QT_PATH%\bin\windeployqt6.exe TetroidNES.exe
TetroidNES.exe