#include <iostream>

using namespace std;
uint16_t Absolute_page_AddressMode_Y(CPU &cpu);
uint16_t Absolute_page_AddressMode_X(CPU &cpu);
uint16_t Absolute(CPU &cpu);
uint16_t Zero_page_AddressMode_Y(CPU &cpu);
uint16_t Zero_page_AddressMode_X(CPU &cpu);
uint16_t Zero_page_AddressMode(CPU &cpu);
uint16_t immediate_AddressMode(CPU &cpu);
uint16_t Indirect_AddressMode(CPU &cpu);
uint16_t Indirect_AddressModeX(CPU &cpu);
uint16_t Indirect_AddressModeY(CPU &cpu);