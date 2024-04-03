# NES Emulator

⚠ THIS PROJECT DOES NOT SUPPORT PIRACY. USE OF THIS EMULATOR OR ITS SOURCE CODE TO ENGAGE IN OR PROMOTE PIRACY IS NOT CONDONED BY ITS AUTHOR. ⚠ 

## CPU Stats for the MOS 6502

Chip created by: MOS Technology 

Clock speed: 1-3 MHz
RAM: 2 kiB
Address size: 16 bits
Word size: 8 bits

Only supports iNES v1.0 ROM files.
Capable of executing all 6502 instructions from NES ROM files. 

## PPU Stats 

(V)RAM: 2 kiB

## Build Instructions

```SH
cmake -S . -B build # build Cmake
cmake --build build --config Release # build project
./Release/output.exe ${path to .nes rom file} # run project
```

## Resources

<a href=https://www.nesdev.org/wiki/Nesdev_Wiki>NES Dev Wiki </a>

<a href=https://www.nesdev.org/obelisk-6502-guide/reference.html#DEX> NES/6502 Instruction set Documenation </a>

<a href=https://www.nesdev.org/wiki/Emulator_tests> Emulator Debug ROM Tests </a>