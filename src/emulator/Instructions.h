#include <iostream>
#include "Computer.h"
using namespace std;

void LDA(uint8_t current_instruction, CPUProcessor &cpu);
void LDX(uint8_t current_instruction, CPUProcessor &cpu);
void LDY(uint8_t current_instruction, CPUProcessor &cpu);
void ADC(uint8_t current_instruction, CPUProcessor &cpu);
void SBC(uint8_t current_instruction, CPUProcessor &cpu);
void CLC(uint8_t current_instruction, CPUProcessor &cpu);
void JMP(uint8_t current_instruction, CPUProcessor &cpu);
void BEQ(uint8_t current_instruction, CPUProcessor &cpu);
void BNE(uint8_t current_instruction, CPUProcessor &cpu);
void CMP(uint8_t current_instruction, CPUProcessor &cpu);
void SEC(uint8_t current_instruction, CPUProcessor &cpu);
void AND(uint8_t current_instruction, CPUProcessor &cpu);
void BCC(uint8_t current_instruction, CPUProcessor &cpu);
void BCS(uint8_t current_instruction, CPUProcessor &cpu);
void BPL(uint8_t current_instruction, CPUProcessor &cpu);
void BVC(uint8_t current_instruction, CPUProcessor &cpu);
void BIT(uint8_t current_instruction, CPUProcessor &cpu);
void ORA(uint8_t current_instruction, CPUProcessor &cpu);
void ROR(uint8_t current_instruction, CPUProcessor &cpu);
void ROL(uint8_t current_instruction, CPUProcessor &cpu);
void JSR(uint8_t current_instruction, CPUProcessor &cpu);