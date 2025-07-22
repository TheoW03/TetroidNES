#pragma once

#include <Emulator/EmulatorUtil.h>
#include <Emulator/Bus.h>

class Execute
{
private:
    CPU cpu;

public:
    Execute(CPU cpu);
    Execute();
    CPU& get_cpu();
    void run();
    renderdata_shared_ptr render();
    int reset_clock();
    void log_Cpu();
    void reset();
    void joypad1(Controller button, const bool isPressed);
    void joypad2(Controller button, const bool isPressed);
};