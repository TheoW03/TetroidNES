# TetroidNES

Nintendo Entertainment System Emulator

## CPU Architecture Specs

instruction set: NES 6502, CISC 
chip creator: MOS technologies and Nintendo
date of release: 1975 (6502), 1984 (NES) 

## CPU Preformance Specs 

Clock speed: 1-3 MHz
RAM: 2 kiB (adress 0x00-0x1fff, 256 bytes is mapped to the stackpointer) it also has 2KiB for VRAM
Address size: 16 bits
Word size: 8 bits
multi purpose registers: 3, 8 bit registers (A,X,Y)
Stack pointer address: 0x100-0x1ff (256 bytes)

Accepted ROM file type: iNES v1.0 (.nes file extension or look for NES in the 1st 4 bytes in the header, or the 7th byte of the header on the 0th and 1st bit is 0)

## Build Instructions

```SH
cmake -S . -B build # build Cmake
cmake --build build --config Release # build project
./Release/output.exe ${path to .nes rom file} # run project
```

## Documentation

Emulator docs: https://github.com/TheoW03/6502Emulator/tree/master/Docs

