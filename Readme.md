# NES emulator

⚠ THIS PROJECT DOES NOT SUPPORT PIRACY. AND IS NOT INTENDED FOR A TOOL OF PIRACY ⚠ 


## cpu stats

RAM: 2 KB

only supports ROM file type iNES v1.0 
capable of executing all 6502 instructions
from NES Rom file 

## Build instructions

This project is built with CMake v3.16 

in terminal

```
cmake -S . -B build
cmake --build build --config Release 
```

the file location of the exe is in Relase/output.exe

to run 
```
./output.exe ${path to .nes rom file}
```
(can only handle NES v1.0 ROM files)