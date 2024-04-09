#include "Bus.h"
#include "Computer.h"



void fetch(CPUProcessor CPU)
{
    CPU.bus.read_8bit(CPU.PC);
}