#include <Emulator/Execute.h>
#include <Emulator/InstructionMap.h>
#include <Emulator/StatusRegister.h>

Execute::Execute(CPU cpu)
{
    this->cpu = cpu;
}

CPU Execute::run()
{
    if (cpu.bus.NMI_interrupt())
    {
        cpu.bus.push_stack8(cpu.status.val);

        cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
        cpu.bus.fetch_next();
        set_interrupt_disabled(1, cpu);
        cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
    }
    cpu.bus.tick();
    auto current_instr = cpu.bus.get_current_instruction();
    if (InstructionValid(current_instr))
    {
        Instruction a = GetInstruction(current_instr);
        a.i(a.addressmode, cpu);

        cpu.error_code = EXIT_SUCCESS;
        return cpu;
    }
    cpu.error_code = EXIT_FAILURE;
    return cpu;
}
std::vector<uint8_t> Execute::render()
{
    return cpu.bus.render_texture({NES_RES_L, NES_RES_W});
}

void Execute::log_Cpu()
{
}
