# Software Structure

to learn how to build this please consildate the readme.md

how does this work from a software stand point


in Main the 1st method is called is init 

init passes in a string for a file of a NES ROM (.nes file extension. by arg)

from init it parses the ROM file. this is done by 2 functions
```C++
vector<uint8_t> file_tobyte_vector(string file_name); //converts ROM into a byte vector
Rom load_rom(vector<uint8_t> instructions); //takes the ROM file header and uses that to figure out where everything is(mappers, PRG start address...) only has support for INES v1.0
```

ROM type
```C++
struct Rom
{
    vector<uint8_t> PRG; //program data. for the cpu (its from 0x8000-0xffff)
    vector<uint8_t> CHR; //Stores tile data for the ppu 
    uint8_t mapper; //mappes. can change the look and feel of the game 
    MirrorType mirror; //enum for if its 4 screen or 2 screen or 1 screen for the ppu
};
```

once the ROM is loaded. it stores all this in the Bus.cpp class and CPU struct 
```C++
struct CPU
{
    uint8_t X_Reg;
    uint8_t Y_Reg; 
    uint8_t A_Reg; //NES's multi purpose registers (they are 3. A,X,Y)
    uint16_t stack_pointer; //Stack
    uint16_t PC; //depreciated. we use bus.program_counter
    uint8_t status; //status reguster. they are 8 different statuses. used for branching 
    Bus bus; //the data bus (its used to communicate data between componets)
};```



```C++
#ifndef BUS_H
#define BUS_H
class Bus
{
private:
    uint8_t v_memory[0x800]; //RAM has 2048 bytes of it. but ROM,PPU VRAM and APU ram exist too
    uint16_t reset_vector;//the location of the RESET vector 
    //compoents
    Rom rom; //ROM
    PPU ppu; //graphics
    APU apu;//Audio
    uint16_t program_counter; 

public:
    int clock_cycles;
    uint8_t stored_instructions[2]; //pipe line stores instructions

    Bus();
    Bus(Rom rom);

    uint8_t get_current_instruction();
    uint8_t fetch_next(); //fetch operation 
    void fill(uint16_t program_counter); //for the Pipe line. it fills up the pipe line with instructions 
    //read/write to memory 
    uint8_t read_8bit(uint16_t address);
    void write_8bit(uint16_t address, uint8_t value);
    uint16_t read_16bit(uint16_t address);
    void write_16bit(uint16_t address, uint16_t value);
    void print_clock(); //debug methid. shows our clock cycles
    uint16_t get_PC(); //retusns the Real PC 
};
#endif
```
the bus class

from then on its a hashmap that points each opCode to a struct instruction 

```C++
using instructionPointer = void (*)(AddressMode, CPU &); //function ptr for each NES instruction

struct Instruction
{
	instructionPointer i;
	AddressMode addressmode; //Address mode type enum. for the 9 address modes of the NES 
};
std::map<uint8_t, Instruction> instructionMap; //hashmap
```

it runs until a BRK is called or an unrecognized instruction.or if it reaches 0xffff

clock cycles here are calculated by each r/w each instruction does 
example: LDA #1 takes 2 clock cycles because it does 2 fetchs