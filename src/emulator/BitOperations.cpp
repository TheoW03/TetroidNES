#include <iostream>
#include "StatusRegister.h"

using namespace std;
uint8_t add(uint8_t a, uint8_t b, uint8_t &carry)
{
	while (b != 0)
	{
		carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
uint8_t mul(uint8_t a, uint8_t b)
{
	int result = 0;

	while (a > 0 && b > 0)
	{
		if (a & 1)
		{				 // if Least significant bit exists
			result += b; // add by shifted left
		}
		a >>= 1;
		b <<= 1; // next bit
	}
	return result;
}

uint8_t sub(uint8_t a, uint8_t b, uint8_t &carry)
{

	while (b != 0)
	{
		carry = (~a) & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
