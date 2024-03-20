# NES Emulator

⚠ THIS PROJECT DOES NOT SUPPORT PIRACY. USE OF THIS EMULATOR OR ITS SOURCE CODE TO ENGAGE IN OR PROMOTE PIRACY IS NOT CONDONED BY ITS AUTHOR. ⚠ 

## CPU Stats for the MOS 6502

Chip created by: MOS Technology

Clock speed: 1-3 MHz
RAM: 2 kB
Address size: 16 bits
Word size: 8 bits

Only supports iNES v1.0 ROM files.
Capable of parsing and executing all 6502 instructions from NES ROM files. 

## PPU Stats 

(V)RAM: 2 kB

## Build Instructions

In the root directory...

```
cmake -S . -B build
cmake --build build --config Release 
./Release/output.exe ${path to .nes rom file}
```