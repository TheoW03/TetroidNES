#include <iostream>

#include "Computer.h"

using namespace std;

// void LDA(uint8_t current_instruction, CPU &cpu);s
void LDA(AddressMode addressType, CPU &cpu);
void LDX(AddressMode addressType, CPU &cpu);
void LDY(AddressMode addressType, CPU &cpu);

void LDX(uint8_t current_instruction, CPU &cpu);
void LDY(uint8_t current_instruction, CPU &cpu);
void ADC(uint8_t current_instruction, CPU &cpu);
void SBC(uint8_t current_instruction, CPU &cpu);
void CLC(uint8_t current_instruction, CPU &cpu);
void JMP(uint8_t current_instruction, CPU &cpu);
void BEQ(uint8_t current_instruction, CPU &cpu);
void BNE(uint8_t current_instruction, CPU &cpu);
void CMP(uint8_t current_instruction, CPU &cpu);
void SEC(uint8_t current_instruction, CPU &cpu);
void AND(uint8_t current_instruction, CPU &cpu);
void BCC(uint8_t current_instruction, CPU &cpu);
void BCS(uint8_t current_instruction, CPU &cpu);
void BPL(uint8_t current_instruction, CPU &cpu);
void BVC(uint8_t current_instruction, CPU &cpu);
void BIT(uint8_t current_instruction, CPU &cpu);
void ORA(uint8_t current_instruction, CPU &cpu);
void ROR(uint8_t current_instruction, CPU &cpu);
void ROL(uint8_t current_instruction, CPU &cpu);
void JSR(uint8_t current_instruction, CPU &cpu);
void CLV(uint8_t current_instruction, CPU &cpu);
void SEI(uint8_t current_instruction, CPU &cpu);
void CLI(uint8_t current_instruction, CPU &cpu);
void SED(uint8_t current_instruction, CPU &cpu);
void CLD(uint8_t current_instruction, CPU &cpu);
void RTI(uint8_t current_instruction, CPU &cpu);
void BMI(uint8_t current_instruction, CPU &cpu);
void BVS(uint8_t current_instruction, CPU &cpu);
void RTS(uint8_t current_instruction, CPU &cpu);
void CPX(uint8_t current_instruction, CPU &cpu);
void CPY(uint8_t current_instruction, CPU &cpu);
void PLP(uint8_t current_instruction, CPU &cpu);
void PHP(uint8_t current_instruction, CPU &cpu);
void EOR(uint8_t current_instruction, CPU &cpu);
void ASL(uint8_t current_instruction, CPU &cpu);
void LSR(uint8_t current_instruction, CPU &cpu);
void DEX(uint8_t current_instruction, CPU &cpu);
void DEY(uint8_t current_instruction, CPU &cpu);
void DEC(uint8_t current_instruction, CPU &cpu);
void TXA(uint8_t current_instruction, CPU &cpu);
void TAX(uint8_t current_instruction, CPU &cpu);
void TYA(uint8_t current_instruction, CPU &cpu);
void TAY(uint8_t current_instruction, CPU &cpu);
void STY(uint8_t current_instruction, CPU &cpu);
void STX(uint8_t current_instruction, CPU &cpu);
void STA(uint8_t current_instruction, CPU &cpu);
void PLA(uint8_t current_instruction, CPU &cpu);
void PHA(uint8_t current_instruction, CPU &cpu);
void TXS(uint8_t current_instruction, CPU &cpu);
void TSX(uint8_t current_instruction, CPU &cpu);
void INC(uint8_t current_instruction, CPU &cpu);
void INX(uint8_t current_instruction, CPU &cpu);
void INY(uint8_t current_instruction, CPU &cpu);
