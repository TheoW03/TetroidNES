# TetroidNES

![ci build][build] ![license][license]

TetroidNES is an open source multi platform NES emulator.

It currently has support for both windows and debian based linux distros

## Build Instructions

glance at the Prerequisites page in the wiki to see what you need to download and compiler
[Prerequisites]

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

[documentation]: <https://github.com/TheoW03/TetroidNES/wiki>
[build]: <https://github.com/TheoW03/TetroidNES/actions/workflows/ci.yml/badge.svg>
[license]: <https://img.shields.io/badge/License-MIT-yellow.svg>
[Prerequisites]: <https://github.com/TheoW03/TetroidNES/wiki/Build-Instructions>