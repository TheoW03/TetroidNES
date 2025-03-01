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
    if (cpu.bus.NMI_interrupt() && !cpu.interrupt.has_value())
    {
        cpu.bus.push_stack8(cpu.status.val);
        qInfo() << "NMI interrupt";
        // printf("%x \n", cpu.bus.get_PC());
        cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
        // printf("the pc %x \n", cpu.bus.get_PC());
        // printf("print");
        cpu.interrupt = 1;
        set_interrupt_disabled(1, cpu);
        cpu.bus.fetch_next();
        // set_interrupt_disabled(1, cpu);
        set_brk(cpu, 1);

        cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
        // cpu.bus.tick();
        // cpu.bus.tick();
    }
    if (cpu.bus.check_error().has_value())
    {
        qCritical() << "ERROR WITH THE CPU" << cpu.bus.check_error().value();
        cpu.error_code = EXIT_FAILURE;

        return cpu;
        // qCritical() << "instruction" << num_to_hexa(current_instr) << "is invalid";
    }
    auto current_instr = cpu.bus.fetch_next();
    // printf("0x%x \n", current_instr);
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

int Execute::reset_clock()
{
    return cpu.bus.reset_clock();
}
void Execute::log_Cpu()
{
    this->cpu.bus.log_ppu();
    std::bitset<8>
        controller1(this->cpu.bus.joy_pad_byte1);
    std::bitset<8>
        controller2(this->cpu.bus.joy_pad_byte2);
    qInfo() << "========CONTROLLER ON EXIT=====";
    qInfo() << "controller byte1: " << controller1.to_string();
    qInfo() << "controller byte2: " << controller2.to_string();
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

void Execute::reset()
{
    // Bus bus = Bus(this->rom, NES_START);
    cpu.bus.fill(cpu.bus.read_16bit(0xfffc));
    // printf("0x%x\n", bus.get_PC());

    // cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;
    cpu.interrupt = {};
}
void Execute::joypad2(Controller button, int isPressed)
{
    // qDebug() << "joypad2";
    cpu.bus.write_controller2(button, isPressed);
}
void Execute::joypad1(Controller button, int isPressed)
{
    // qDebug() << "joypad1";

    cpu.bus.write_controller1(button, isPressed);
}
