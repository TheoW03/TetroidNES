#include <iostream>
#include "Computer.h"

uint8_t add(uint8_t a, uint8_t b, CPU &cpu, uint8_t &carry);

uint8_t mul(uint8_t a, uint8_t b);

uint8_t sub(uint8_t a, uint8_t b, CPU &cpu, uint8_t &carry);
uint8_t decimal_add(uint8_t a, uint8_t b, CPU &cpu, uint8_t &carry);
uint8_t decimal_sub(uint8_t a, uint8_t b, CPU &cpu, uint8_t &carry);

uint8_t leftRotate(uint8_t n, uint8_t d);
uint8_t rightRotate(uint8_t n, uint8_t d);