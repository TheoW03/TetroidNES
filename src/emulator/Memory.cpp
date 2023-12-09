#include <iostream>


using namespace std;


static int memory[1048576];

	
void Write_mem(int address, int num){
	address = address * 32;
	for (int i = 31; i >= 0; --i) {
        	memory[(i + address)*8] = (num >> i) & 1;
    	}

}
int Read_mem(int address){
	address = address * 32;
	int decimalValue = 0;
	for (int i = 31; i >= 0; i--) {
		if(memory[(i + address)*8] == 1) decimalValue += (1 << i);
    	}
	return decimalValue;
}

void Load_instructions(int instructions[], int size){
	for(int i = 0; i < size; i++){
		Write_mem(i, instructions[i]);
	}
	
}
