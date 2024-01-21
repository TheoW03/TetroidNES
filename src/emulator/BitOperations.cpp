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
	return a;
}
uint8_t mul(uint8_t a, uint8_t b)
{
	int result = 0;

	while (a > 0 && b > 0)
	{
		if (a & 1)		 // if Least significant bit exists
			result += b; // add by shifted left
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
uint8_t decimal_sub(uint8_t a, uint8_t b, CPUProcessor &cpu, uint8_t &carry)
{
	uint8_t lo_b = b & 0b1111;
	uint8_t hi_b = (b >> 4) & 0b1111;
	if (lo_b > 0xa || hi_b > 0xa)
	{
		cout << "test" << endl;
		return 0;
	}
	uint8_t lo_a = a & 0b1111;
	uint8_t hi_a = (a >> 4) & 0b1111;
	if (hi_a > 0xa || lo_a > 0xa)
	{
		cout << "test" << endl;
		return 0;
	}
	uint8_t lo_diff = sub(lo_a, lo_b, cpu, carry);
	if ((int8_t)lo_diff < 0)
		lo_diff += 10;
	uint8_t hi_diff = sub(hi_a, hi_b, cpu, carry);
	hi_diff = sub(hi_diff, 1, cpu, carry);
	if ((int8_t)hi_diff < 0)
		hi_diff += 11;
	return (hi_diff << 4) | lo_diff;
}

uint8_t decimal_add(uint8_t a, uint8_t b, CPUProcessor &cpu, uint8_t &carry)
{
	uint8_t lo_b = b & 0b1111;
	uint8_t hi_b = (b >> 4) & 0b1111;
	if (lo_b >= 0xa || hi_b >= 0xa)
		return 0;
	uint8_t lo_a = a & 0b1111;
	uint8_t hi_a = (a >> 4) & 0b1111;
	if (hi_a >= 0xa || lo_a >= 0xa)
		return 0;
	// uint8_t carry = 0;
	uint8_t lo_Sum = add(lo_a, lo_b, cpu, carry);
	// uint8_t lo_Sum = lo_a + lo_b;
	uint8_t c = lo_Sum - (lo_Sum % 10);
	uint8_t this_carry = c / 10;
	lo_Sum %= 10;
	uint8_t hi_sum = hi_a + hi_b + this_carry;
	hi_sum = add(hi_sum, this_carry, cpu, carry);
	carry = this_carry;
	return (hi_sum << 4) | lo_Sum;
}


uint8_t leftRotate(uint8_t n, uint8_t d)
{

    return (n << d) | (n >> (8 - d));
}

uint8_t rightRotate(uint8_t n, uint8_t d)
{
    return (n >> d) | (n << (8 - d));
}