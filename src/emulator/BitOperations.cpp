#include <iostream>
#include "StatusRegister.h"

using namespace std;
uint8_t add(uint8_t a, uint8_t b)
{
	int carry = 0;
	while (b != 0)
	{
		carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	cout << carry << endl;
	return a;
}
int mul(int a, int b)
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

int sub(int a, int b)
{

	int carry = 0;
	while (b != 0)
	{
		carry = (~a) & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
