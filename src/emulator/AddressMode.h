#include <iostream>

using namespace std;
uint16_t Absolute_page_AddressMode_Y(CPUProcessor &cpu);
uint16_t Absolute_page_AddressMode_X(CPUProcessor &cpu);
uint16_t Absolute(CPUProcessor &cpu);
uint16_t Zero_page_AddressMode_Y(CPUProcessor &cpu);
uint16_t Zero_page_AddressMode_X(CPUProcessor &cpu);
uint16_t Zero_page_AddressMode(CPUProcessor &cpu);
uint16_t immediate_AddressMode(CPUProcessor &cpu);
uint16_t Indirect_AddressMode(CPUProcessor &cpu);
uint16_t Indirect_AddressModeX(CPUProcessor &cpu);
uint16_t Indirect_AddressModeY(CPUProcessor &cpu);