#include <Emulator/Computer.h>

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
};
#endif