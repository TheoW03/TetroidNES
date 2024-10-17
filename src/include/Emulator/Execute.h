#include <Emulator/Computer.h>

#define NES_START 0x8000

#ifndef EXECUTE_H
#define EXECUTE_H
class Execute
{
private:
    CPU cpu;

public:
    Execute(CPU cpu);
    CPU run();
    std::vector<uint8_t> render();
    void log_Cpu();
};
#endif