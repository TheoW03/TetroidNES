#include <iostream>


using namespace std;


static int memory[128000];

	
void Write_mem(int address, int num){
	for (int i = 31; i >= 0; --i) {
        	memory[(i + address)*8] = (num >> i) & 1;
    	}

}
int Read_mem(int address){
	int decimalValue = 0;
	for (int i = 31; i >= 0; i--) {
		if(memory[(i + address)*8] == 1) decimalValue += (1 << i);
    	}
	return decimalValue;
}
