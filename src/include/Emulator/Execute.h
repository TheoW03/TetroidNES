#include <Emulator/Computer.h>

class Execute
{
private:
    CPU cpu;

public:
    Execute(CPU cpu);
    CPU run();
};