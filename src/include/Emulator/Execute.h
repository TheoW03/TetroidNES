#include <Emulator/EmulatorUtil.h>

#define NES_START 0x8000

#ifndef EXECUTE_H
#define EXECUTE_H
class Execute
{
private:
    CPU cpu;

public:
    Execute(CPU cpu);
    Execute();
    CPU run();
    std::vector<uint8_t> render();
    int reset_clock();
    void log_Cpu();
    void joypad1(Controller button, int isPressed);
    void joypad2(Controller button, int isPressed);
};
#endif