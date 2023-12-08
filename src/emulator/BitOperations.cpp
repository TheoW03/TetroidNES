#include <iostream>

using namespace std;
int add(int a, int b){
	int sum = 0;
	int carry = 0;
	while(b != 0){
		int carry = a & b;
		a = a ^ b;
        	b = carry << 1;
	}
	return a;
}
int mul(int a, int b){
	int result = 0;
    	
	while (a > 0 && b > 0)
    	{
        	if (a & 1)
       		 {                // if Least significant bit exists
       		     result += b; // add by shifted left
       		 }
       		 a >>= 1;
       		 b <<= 1; // next bit
    	}
    	return result;	
	
}

int sub(int a, int b)
{

	int sum = 0;
	int carry = 0;
	while(b != 0){
		int carry = (~a) & b;
		a = a ^ b;
        	b = carry << 1;
	}
	return a;


}	
