# Address Mode

since the 6502 is CISC each instruction has different ways of manupilating data. each instruction
can have multiple address modes

Immediate: just returns the PC. 

zero page x,y: 8 bit ptr with offsets of x,y 

Absolute x,y: 16 bit ptr with offsets x,y

indirect x,y; 16 bit ptr with the LSB in it can offset with x,y


more info: https://www.nesdev.org/obelisk-6502-guide/addressing.html#IDX

