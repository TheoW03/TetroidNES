#include <iostream>

using namespace std;

uint16_t immediate_address_mode(CPU &cpu);

uint16_t zero_page_address_mode(CPU &cpu);
uint16_t zero_page_address_mode_X(CPU &cpu);
uint16_t zero_page_address_mode_Y(CPU &cpu);

uint16_t absolute(CPU &cpu);
uint16_t absolute_page_address_mode_X(CPU &cpu);
uint16_t absolute_page_address_mode_Y(CPU &cpu);

uint16_t indirect_address_mode(CPU &cpu);
uint16_t indirect_address_mode_X(CPU &cpu);
uint16_t indirect_address_Mode_Y(CPU &cpu);
