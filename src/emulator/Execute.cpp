#include <bitset>

#include <QDebug>

#include <Emulator/Execute.h>
#include <Emulator/InstructionMap.h>
#include <Emulator/StatusRegister.h>
#include <Qt/utils/util.h>

#define NMI_VECTOR 0xfffa
#define IRQ_VECTOR 0xfffc

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

        cpu.bus.fill(cpu.bus.read_16bit(NMI_VECTOR));
        // cpu.bus.tick();
        // cpu.bus.tick();
    }
    if (cpu.interrupt.has_value())
    {
        // qInfo() << "interrupt: " << num_to_hexa(cpu.bus.get_PC());
    }
    if (cpu.bus.check_error().has_value())
    {
        qCritical() << "ERROR WITH THE CPU" << cpu.bus.check_error().value();
        cpu.error_code = EXIT_FAILURE;

        return cpu;
        // qCritical() << "instruction" << num_to_hexa(current_instr) << "is invalid";
    }
    auto current_instr = cpu.bus.fetch_next();
    // exit(EXIT_FAILURE);q
    if (InstructionValid(current_instr))
    {

        Instruction a = GetInstruction(current_instr);
        a.i(a.addressmode, cpu);
        cpu.error_code = EXIT_SUCCESS;
        // joypad1(Controller::LEFT, 0);

        return cpu;
    }
    qCritical() << "instruction" << num_to_hexa(current_instr) << "is invalid";

    qInfo() << "potential error with the cpu";

    cpu.error_code = EXIT_FAILURE;

    return cpu;
}
std::vector<uint8_t>* Execute::render()
{
    return cpu.bus.render_texture();
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
    qInfo() << "========CONTROLLER ON EXIT====="
            << "\ncontroller byte1: " << controller1.to_string()
            << "\ncontroller byte2: " << controller2.to_string()
            << "\n=====CPU on quit======"
            << "\nA register on exit: " << this->cpu.A_Reg
            << "\nX register on exit: " << this->cpu.X_Reg
            << "\nY register on exit: " << this->cpu.Y_Reg
            << "\nPC on exit: 0x" << num_to_hexa(this->cpu.bus.get_PC())
            << "\nStack pointer " << num_to_hexa(this->cpu.bus.get_stack_pointer());
    std::bitset<7>
        status(this->cpu.status.val);

    qInfo() << "====STATUS REGTISTER BITS===="
            << "\nCarry: " << this->cpu.status.C
            << "\nZero: " << this->cpu.status.Z
            << "\nBreak: " << this->cpu.status.B
            << "\nInterrupt disabled: " << this->cpu.status.I
            << "\ndecimal mode: " << this->cpu.status.D
            << "\nOverflow: " << this->cpu.status.V
            << "\nNegative: " << this->cpu.status.N
            << "\nvalue: " << status.to_string()
            << "\n==============="
            << "\nclock cycles: " << this->cpu.bus.clock_cycles
            << "\n";
}

void Execute::reset()
{
    // Bus bus = Bus(this->rom, NES_START);
    cpu.bus.fill(cpu.bus.read_16bit(IRQ_VECTOR));

    // cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;
    cpu.interrupt = {};
}
void Execute::joypad2(Controller button, const bool isPressed)
{
    // qDebug() << "joypad2";
    cpu.bus.write_controller2(button, isPressed);
}
void Execute::joypad1(Controller button, const bool isPressed)
{
    // qDebug() << "joypad1";

    cpu.bus.write_controller1(button, isPressed);
}
