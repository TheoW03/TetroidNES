# TetroidNES

![ci build][build] ![license][license]

TetroidNES is an open source multi platform NES emulator.

It currently has support for both Windows and debian based linux distros

## Build Instructions

glance at the Prerequisites page in the wiki to see what you need to download and compiler

[Prerequisites]

once you do that run this 

```SH
cmake  -DCMAKE_PREFIX_PATH=~/path/to/Qt/6.8.0/gcc_64/ -DCMAKE_CXX_FLAGS="-std=c++17"  -S . -B build # this builds the CMake

cmake --build build --config Release # builds project 

./build.sh # build script if this is too muc 

./build/TetroidNES # runs project 

```

check out the ./log/ directory to see logs

## Documentation

[documentation]

a special thanks to all who helped me make this


<a href="https://github.com/TheoW03/TetroidNES/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=TheoW03/TetroidNES" alt="including wolfymyth. helped alot"/>
</a>

[documentation]: <https://github.com/TheoW03/TetroidNES/wiki>
[build]: <https://github.com/TheoW03/TetroidNES/actions/workflows/ci.yml/badge.svg>
[license]: <https://img.shields.io/badge/License-MIT-yellow.svg>
[Prerequisites]: <https://github.com/TheoW03/TetroidNES/wiki/Build-Instructions>