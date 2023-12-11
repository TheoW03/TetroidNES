#include <iostream>
#include "BitOperations.h"
#include "Memory.h"

using namespace std;

#define LDA_IM 0xA9 //sample fmt

//PC
int PC = 0;

//registers
int A_Reg = 0;
int X_Reg = 0;
int Y_reg = 0;



// flags
int status = 0;
int N = 0;
int C = 0;
int D = 0;
int B = 0;
int V = 0;

int param = 0;
int current_instruction = 0;
	

void fetch(){
	current_instruction = Read_mem(PC);	
	PC = add(PC, 1); 		
}
void decode(){
	switch(current_instruction){
		case LDA_IM:
		{	
			param = Read_mem(PC);
			PC = add(PC,1);
		}
	}

	
}
void execute(){

	switch(current_instruction){
		case LDA_IM:
		{	
			A_Reg = param;
			if(A_Reg == 0){
				status &= 0b00000010;
			}else{
				status &= 0b11111101;	
			}
			if(A_Reg & 0b10000000 != 0){
				status = status | 0b10000000;	
			}else{
				status = status & 0b01111111;	
			}
		}

	}
	
}

void store(){
}
void run(){
	while(1){
		fetch();
		decode();
		execute();
		store();
	}
}


