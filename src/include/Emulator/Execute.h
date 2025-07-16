#pragma once

#include <Emulator/EmulatorUtil.h>
#include <Emulator/Bus.h>

#define NES_START 0x8000

class Execute
{
private:
    CPU cpu;

public:
    Execute(CPU cpu);
    Execute();
    CPU run();
    renderdata_shared_ptr render();
    int reset_clock();
    void log_Cpu();
    void reset();
    void joypad1(Controller button, const bool isPressed);
    void joypad2(Controller button, const bool isPressed);
};