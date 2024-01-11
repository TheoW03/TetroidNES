#include <iostream>
#include "Computer.h"
using namespace std;

void LDA(uint8_t current_instruction, CPUProcessor &cpu);
void LDX(uint8_t current_instruction, CPUProcessor &cpu);
void LDY(uint8_t current_instruction, CPUProcessor &cpu);
void ADC(uint8_t current_instruction, CPUProcessor &cpu);
void SBC(uint8_t current_instruction, CPUProcessor &cpu);