#include <Emulator/component_registers.h>

ComponentRegister::ComponentRegister()
{
}

void ComponentRegister::write_8bit(uint8_t value)
{
}

uint8_t ComponentRegister::read_8bit()
{
    return 0;
}

void ComponentRegister::write_16bit(uint16_t value)
{
}

uint16_t ComponentRegister::read_16bit()
{
    return 0;
}

void ComponentRegister::reset_latch()
{
}
int ComponentRegister::get_bit(int inudex)
{
    return 0;
}

void ComponentRegister::set_bit(int index, int toggle)
{
}
