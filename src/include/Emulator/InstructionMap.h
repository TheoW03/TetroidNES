#include <Emulator/Instructions.h>

using InstructionPointer = void (*)(AddressMode, CPU&);

// Instruction Pointers
namespace InstFunc {
    constexpr const InstructionPointer LDA_PTR(LDA);
    constexpr const InstructionPointer LDX_PTR(LDX);
    constexpr const InstructionPointer LDY_PTR(LDY);
    constexpr const InstructionPointer ADC_PTR(ADC);
    constexpr const InstructionPointer SBC_PTR(SBC);
    constexpr const InstructionPointer AND_PTR(AND);
    constexpr const InstructionPointer ORA_PTR(ORA);
    constexpr const InstructionPointer ROL_PTR(ROL);
    constexpr const InstructionPointer ROR_PTR(ROR);
    constexpr const InstructionPointer STX_PTR(STX);
    constexpr const InstructionPointer STY_PTR(STY);
    constexpr const InstructionPointer STA_PTR(STA);
    constexpr const InstructionPointer LSR_PTR(LSR);
    constexpr const InstructionPointer ASL_PTR(ASL);
    constexpr const InstructionPointer EOR_PTR(EOR);
    constexpr const InstructionPointer DEX_PTR(DEX);
    constexpr const InstructionPointer DEY_PTR(DEY);
    constexpr const InstructionPointer DEC_PTR(DEC);
    constexpr const InstructionPointer INX_PTR(INX);
    constexpr const InstructionPointer INY_PTR(INY);
    constexpr const InstructionPointer INC_PTR(INC);
    constexpr const InstructionPointer CLC_PTR(CLC);
    constexpr const InstructionPointer SEC_PTR(SEC);
    constexpr const InstructionPointer CLD_PTR(CLD);
    constexpr const InstructionPointer SED_PTR(SED);
    constexpr const InstructionPointer SEI_PTR(SEI);
    constexpr const InstructionPointer CLI_PTR(CLI);
    constexpr const InstructionPointer CLV_PTR(CLV);
    constexpr const InstructionPointer RTI_PTR(RTI);
    constexpr const InstructionPointer JMP_PTR(JMP);
    constexpr const InstructionPointer CMP_PTR(CMP);
    constexpr const InstructionPointer CPY_PTR(CPY);
    constexpr const InstructionPointer CPX_PTR(CPX);
    constexpr const InstructionPointer BIT_PTR(BIT);
    constexpr const InstructionPointer BEQ_PTR(BEQ);
    constexpr const InstructionPointer BNE_PTR(BNE);
    constexpr const InstructionPointer BCC_PTR(BCC);
    constexpr const InstructionPointer BCS_PTR(BCS);
    constexpr const InstructionPointer BPL_PTR(BPL);
    constexpr const InstructionPointer BMI_PTR(BMI);
    constexpr const InstructionPointer BVC_PTR(BVC);
    constexpr const InstructionPointer BVS_PTR(BVS);
    constexpr const InstructionPointer JSR_PTR(JSR);
    constexpr const InstructionPointer RTS_PTR(RTS);
    constexpr const InstructionPointer PLP_PTR(PLP);
    constexpr const InstructionPointer PHP_PTR(PHP);
    constexpr const InstructionPointer PLA_PTR(PLA);
    constexpr const InstructionPointer PHA_PTR(PHA);
    constexpr const InstructionPointer TAX_PTR(TAX);
    constexpr const InstructionPointer TXA_PTR(TXA);
    constexpr const InstructionPointer TXS_PTR(TXS);
    constexpr const InstructionPointer TSX_PTR(TSX);
    constexpr const InstructionPointer TYA_PTR(TYA);
    constexpr const InstructionPointer TAY_PTR(TAY);
    constexpr const InstructionPointer BRK_PTR(BRK);
    constexpr const InstructionPointer NOP_PTR(NOP);
}

struct Instruction
{
    InstructionPointer i;
    AddressMode addressmode;
};

const Instruction& GetInstruction(const uint8_t &opcode);
const bool InstructionValid(const uint8_t &opcode);