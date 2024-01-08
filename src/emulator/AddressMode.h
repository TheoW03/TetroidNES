#include <iostream>

using namespace std;
uint16_t Absolute_page_AddressMode_Y(uint16_t PC, uint8_t Y_REG);
uint16_t Absolute_page_AddressMode_X(uint16_t PC, uint8_t X_REG);
uint16_t Absolute(uint16_t PC);
uint16_t Zero_page_AddressMode_Y(uint16_t PC, uint8_t Y_REG);
uint16_t Zero_page_AddressMode_X(uint16_t PC, uint8_t X_REG);
uint16_t Zero_page_AddressMode(uint16_t PC);
uint16_t immediate_AddressMode(uint16_t PC);