#include <iostream>

#include "Computer.h"

// void LDA(AddressMode addressType, CPU &cpu);s
void LDA(AddressMode addressType, CPU &cpu);
void LDX(AddressMode addressType, CPU &cpu);
void LDY(AddressMode addressType, CPU &cpu);

void ADC(AddressMode addressType, CPU &cpu);
void SBC(AddressMode addressType, CPU &cpu);
void CLC(AddressMode addressType, CPU &cpu);
void JMP(AddressMode addressType, CPU &cpu);
void BEQ(AddressMode addressType, CPU &cpu);
void BNE(AddressMode addressType, CPU &cpu);
void CMP(AddressMode addressType, CPU &cpu);
void SEC(AddressMode addressType, CPU &cpu);
void AND(AddressMode addressType, CPU &cpu);
void BCC(AddressMode addressType, CPU &cpu);
void BCS(AddressMode addressType, CPU &cpu);
void BPL(AddressMode addressType, CPU &cpu);
void BVC(AddressMode addressType, CPU &cpu);
void BIT(AddressMode addressType, CPU &cpu);
void ORA(AddressMode addressType, CPU &cpu);
void ROR(AddressMode addressType, CPU &cpu);
void ROL(AddressMode addressType, CPU &cpu);
void JSR(AddressMode addressType, CPU &cpu);
void CLV(AddressMode addressType, CPU &cpu);
void SEI(AddressMode addressType, CPU &cpu);
void CLI(AddressMode addressType, CPU &cpu);
void SED(AddressMode addressType, CPU &cpu);
void CLD(AddressMode addressType, CPU &cpu);
void RTI(AddressMode addressType, CPU &cpu);
void BMI(AddressMode addressType, CPU &cpu);
void BVS(AddressMode addressType, CPU &cpu);
void RTS(AddressMode addressType, CPU &cpu);
void CPX(AddressMode addressType, CPU &cpu);
void CPY(AddressMode addressType, CPU &cpu);
void PLP(AddressMode addressType, CPU &cpu);
void PHP(AddressMode addressType, CPU &cpu);
void EOR(AddressMode addressType, CPU &cpu);
void ASL(AddressMode addressType, CPU &cpu);
void LSR(AddressMode addressType, CPU &cpu);
void DEX(AddressMode addressType, CPU &cpu);
void DEY(AddressMode addressType, CPU &cpu);
void DEC(AddressMode addressType, CPU &cpu);
void TXA(AddressMode addressType, CPU &cpu);
void TAX(AddressMode addressType, CPU &cpu);
void TYA(AddressMode addressType, CPU &cpu);
void TAY(AddressMode addressType, CPU &cpu);
void STY(AddressMode addressType, CPU &cpu);
void STX(AddressMode addressType, CPU &cpu);
void STA(AddressMode addressType, CPU &cpu);
void PLA(AddressMode addressType, CPU &cpu);
void PHA(AddressMode addressType, CPU &cpu);
void TXS(AddressMode addressType, CPU &cpu);
void TSX(AddressMode addressType, CPU &cpu);
void INC(AddressMode addressType, CPU &cpu);
void INX(AddressMode addressType, CPU &cpu);
void INY(AddressMode addressType, CPU &cpu);
