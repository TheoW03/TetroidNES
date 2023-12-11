#include <iostream>
#include "BitOperations.h"
#include "Memory.h"

using namespace std;

#define LDA_IM 0xA9 //sample fmt
#define LDX_IM 0xA2
#define LDY_IM 0xA0
#define TAX 0xAA
#define TAY 0xA8
//PC
int PC = 0;

//registers
int A_Reg = 0;
int X_Reg = 0;
int Y_Reg = 0;



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
		case TAX: case TAY: 
		default:
		{	
			param = Read_mem(PC);
		 	PC = add(PC,1);
		}
	}

	
}
void set_status_Z(int val){
	if(val == 0){
		status |= 0b00000010;
	}else{
		status &= 0b11111101;	
	}
	
}
void set_status_N(int val){
	if(val & 0b10000000 != 0){
		status = status | 0b10000000;	
	}else{
		status = status & 0b01111111;	
	}
}
void set_status_O(int val){
	
}

void execute(){

	switch(current_instruction){
		case LDA_IM:
		{
			A_Reg = param;
			set_status_Z(A_Reg);
			set_status_N(A_Reg);
		}
		case LDX_IM:
		{
			X_Reg = param;
			set_status_Z(X_Reg);
			set_status_N(X_Reg);
		}
		case LDY_IM:
		{
			Y_Reg = param;
			set_status_Z(Y_Reg);
			set_status_N(Y_Reg);
		}
		case TAX:
		{
			X_Reg = A_Reg;
			set_status_Z(X_Reg);
			set_status_N(X_Reg);
		}
		case TAY:
		{
			Y_Reg = A_Reg;
			set_status_Z(Y_Reg);
			set_status_N(Y_Reg);
		}



	}
	
}

void store(){
}
void CPU_status(){
	cout <<"A: " << A_Reg << endl;
	cout << "X: " << X_Reg << endl;
	cout << "Y: " << Y_Reg << endl;
	
}
void run(){
	
		fetch();
		decode();
		execute();
		store();
		CPU_status();
	
}


