# TetroidNES

![license][license] ![ci build][build] ![repo size][repo-size]

TetroidNES is an open source NES emulator targeted to windows.

It currently has support for windows. and as of writting this it can execute any NES rom so long as it uses no graphics

## Build Instructions

I recomend installing cc65 NES assembler if you want a way to run and build the unit tests

[cc65 download]

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
# downlaode page: 

# builds the Test Roms 
. .\buildTestRoms.ps1  
build 
# run Unit Tests
./Debug/Emulator_tests.exe
```

## Documentation

[documentation]

a special thanks to all who helped me make this

[cc65 download]: https://cc65.github.io/
[documentation]: <https://github.com/TheoW03/6502Emulator/tree/master/Docs>
[build]: <https://github.com/TheoW03/TetroidNES/actions/workflows/ci.yml/badge.svg>
[repo-size]: <https://img.shields.io/github/repo-size/TheoW03/TetroidNES>
[license]: <https://img.shields.io/badge/License-MIT-yellow.svg>
