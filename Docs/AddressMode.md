# Address Mode

since the 6502 is CISC each instruction has different ways of where it can get data from. 
which is an address mode.
each instruction can have multiple address modes. but not multiple 
of the same address mode. the amount of address modes for each instruction varies
consult the 6502 instruction set documentation for more info

Implied: for instructions that dont manupilate data (like CLC)

Relative: for BEQ, BNE. it adds on to the PC +/- 128

Immediate: just returns the PC. 

zero page x,y: 8 bit ptr with offsets of x,y 

Absolute x,y: 16 bit ptr with offsets x,y

indirect x,y; 16 bit ptr with the LSB in it can offset with x,y

https://www.nesdev.org/obelisk-6502-guide/addressing.html#IDX

