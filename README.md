# TetroidNES

open source NES emulator
targeted to windows

## Build Instructions

```SH
# build Cmake
cmake -S . -B build 

# build amd run project
cmake --build build --config Release
./Release/tetroid.exe ${path to .nes rom file} 

# build and run unit tests
cmake --build build
# make sure TestRoms are up to date

# TODO  install cc65 assembler and add it to your PATH/bin directory 
# if you want buildTestRoms.ps1 to work
# downlaode page: https://cc65.github.io/

# builds the Test Roms 
. .\buildTestRoms.ps1 
build 
# run Unit Tests
./Debug/Emulator_tests.exe
```

## Documentation

Emulator docs: https://github.com/TheoW03/6502Emulator/tree/master/Docs

a special thanks to all who halped me make this