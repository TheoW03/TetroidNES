#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
using namespace std;
int main(int argc, char *argv[])
{
	int a[2] = {0xA9, 0x5};
 	Load_instructions(a,sizeof(a));
	run();
	cout << "compiled" << endl;

}
