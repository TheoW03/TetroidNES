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
    // std::bitset<7> status(this->cpu.status.val);
    // std::cout << status << std::endl;

    if (cpu.bus.NMI_interrupt() && check_brk(cpu) == 0)
    {
        cpu.bus.push_stack8(cpu.status.val);
        qInfo() << "NMI interrupt, should be rendering";
        printf("%x \n", cpu.bus.get_PC());
        cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
        cpu.bus.fetch_next();
        set_interrupt_disabled(1, cpu);
        set_brk(cpu, 1);
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
        // cpu.bus.print_ppu();
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
    this->cpu.bus.log_ppu();
    qInfo() << "=====CPU on quit======";
    qInfo() << "A register on exit: " << this->cpu.A_Reg;
    qInfo() << "X register on exit: " << this->cpu.X_Reg;
    qInfo() << "Y register on exit: " << this->cpu.Y_Reg;
    qInfo() << "PC on exit: 0x" << num_to_hexa(this->cpu.bus.get_PC());
    qInfo() << "Stack pointer " << num_to_hexa(this->cpu.bus.get_stack_pointer());
    std::bitset<7>
        status(this->cpu.status.val);

    qInfo() << "====STATUS REGTISTER BITS====";
    qInfo() << "Carry: " << this->cpu.status.C;
    qInfo() << "Zero: " << this->cpu.status.Z;
    qInfo() << "Break: " << this->cpu.status.B;
    qInfo() << "Interrupt disabled: " << this->cpu.status.I;
    qInfo() << "decimal mode: " << this->cpu.status.D;

    qInfo() << "Overflow: " << this->cpu.status.V;
    qInfo() << "Negative: " << this->cpu.status.N;
    qInfo() << "value: " << status.to_string();
    qInfo() << "===============";
    qInfo() << "clock cycles: " << this->cpu.bus.clock_cycles;
    qInfo() << "";
}
