#include <iostream>
#include "BitOperations.h"
#include "Memory.h"

using namespace std;

#define LDA_IM 0xA9 //sample fmt

int PC = 0;

//registers
int A_Reg = 0;
int X_Reg = 0;
int Y_reg = 0;
int status = 0;


// flags
int N = 0;
int C = 0;
int D = 0;
int B = 0;
int V = 0;
	
void ALU(int opcode, int a, int b){
}
void fetch(){
}
void decode(){
}
void execute(){
}
void store(){
}
void run(){	
}


