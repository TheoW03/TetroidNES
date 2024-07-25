# Different Components 


the NES's differnet components 

CPU: central processing unit. its the brains of this computer.

PPU: picture processing unit. or a GPU. it handles the graphics
The NES GPU. unlike modern GPU's uses no linear algebra or parelization
it has no instructions. but you communicate to it by reading/writting to 0x2000-0x2007

APU: audio processing unit, sound. like the PPU. but you writting too 0x4000-0x4200
 
 all these communicate through the Bus. 
 there is an address bus for addresses (that can get up to 16 bits)
 and a Data bus for communication between components