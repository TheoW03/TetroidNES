#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
using namespace std;
int main(int argc, char *argv[])
{
	Write_mem(0, -10);
	cout << Read_mem(0) << endl;
	Write_mem(1, -20);	
	cout << Read_mem(0) << endl;
    	// cout << "hello world" << endl;	
	
	 
	
}








