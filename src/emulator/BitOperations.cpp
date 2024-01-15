#include <iostream>
#include "StatusRegister.h"
#include <bitset>
#include "Computer.h"
#include "StatusRegister.h"
using namespace std;
uint8_t add(uint8_t a, uint8_t b, CPUProcessor &cpu, uint8_t &carry)
{
	uint8_t c_in = 0;
	uint8_t c_out = 0;

	while (b != 0)
	{
		c_in = carry;
		// cout << "cin: " << bitset<8>(carry) << endl;
		carry = a & b;
		a = a ^ b;
		b = carry << 1;
		// cout << "cout: " << bitset<8>(carry) << endl;
		c_out = carry;
		set_overflow(c_in, c_out, cpu);
	}
	// if (carry != 0)
	// 	a++;
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

uint8_t sub(uint8_t a, uint8_t b, CPUProcessor &cpu, uint8_t &carry)
{
	uint8_t c_in = 0;
	uint8_t c_out = 0;

	while (b != 0)
	{
		c_in = carry;
		// cout << "cin: " << bitset<8>(carry) << endl;

		carry = (~a) & b;
		a = a ^ b;
		b = carry << 1;
		c_out = carry;
		// cout << "cout: " << bitset<8>(carry) << endl;

		set_overflow(c_in, c_out, cpu);
	}
	// carry = ~carry;
	carry = carry == 0;
	// cout << "result: " << bitset<8>(carry) << endl;
	// if (carry != 0)
	// 	carry--;

	return a;
}
