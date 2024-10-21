#include <Emulator/Execute.h>
#include <Emulator/InstructionMap.h>
#include <Emulator/StatusRegister.h>
#include <QDebug>
#include <Qt/util.h>
#include <bitset>
Execute::Execute(CPU cpu)
{
    this->cpu = cpu;
    printf("%x \n", this->cpu.bus.get_PC());
    // std::cout << this->cpu.bus.get_PC() << std::end
}

Execute::Execute()
{
}
CPU Execute::run()
{
    if (cpu.bus.NMI_interrupt())
    {
        cpu.bus.push_stack8(cpu.status.val);
        qInfo() << "NMI interrupt, should be rendering";
        cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
        cpu.bus.fetch_next();
        set_interrupt_disabled(1, cpu);
        cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
    }
    if (cpu.bus.check_error().has_value())
    {
        qCritical() << "ERROR WITH THE CPU" << cpu.bus.check_error().value();
        cpu.error_code = EXIT_FAILURE;

        return cpu;
        // qCritical() << "instruction" << num_to_hexa(current_instr) << "is invalid";
    }
    cpu.bus.tick();
    auto current_instr = cpu.bus.fetch_next();
    if (InstructionValid(current_instr))
    {
        Instruction a = GetInstruction(current_instr);
        a.i(a.addressmode, cpu);
        cpu.error_code = EXIT_SUCCESS;
        return cpu;
    }
    qCritical() << "instruction" << num_to_hexa(current_instr) << "is invalid";

    qInfo() << "potential error with the cpu";

    cpu.error_code = EXIT_FAILURE;
    return cpu;
}
std::vector<uint8_t> Execute::render()
{
    return cpu.bus.render_texture({NES_RES_L, NES_RES_W});
}

void Execute::log_Cpu()
{
    qInfo() << "CPU on quit";
    qInfo() << "A register on exit: " << this->cpu.A_Reg;
    qInfo() << "X register on exit: " << this->cpu.X_Reg;
    qInfo() << "Y register on exit: " << this->cpu.Y_Reg;
    qInfo() << "PC on exit: 0x: " << num_to_hexa(this->cpu.bus.get_PC());
    std::bitset<7> status(this->cpu.status.val);
    qInfo() << "status: 0b" << status.to_string();
}
