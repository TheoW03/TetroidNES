# NES emulator

⚠ THIS PROJECT DOES NOT SUPPORT PIRACY. AND IS NOT INTENDED FOR A TOOL OF PIRACY ⚠ 


## cpu stats

RAM: 2 KB

only supports ROM file type iNES v1.0 
capable of executing all 6502 instructions
from NES Rom file 

## Build instructions

```
cmake -S . -B build
cmake --build build --config Release 
./Release/output.exe ${path to .nes rom file}
