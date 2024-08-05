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
./Debug/Emulator_tests.exe
```

## Documentation

Emulator docs: https://github.com/TheoW03/6502Emulator/tree/master/Docs

a special thanks to all who halped me make this