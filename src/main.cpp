#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
#include <bitset>
#include "SetupSFML.h"
#include <cassert>
using namespace std;
// #define NAME_OF(v) #v

// void *colored_pixels(void *arg)
// {
// 	vector<uint8_t> instructions = {0xa0,
// 									0x00, 0xc8, 0xc0, 0xff, 0xee, 0x05,
// 									0x00, 0xac, 0x05, 0x00, 0xc0, 0x30, 0xee, 0x00, 0x02, 0xee, 0x03, 0x02,
// 									0xee, 0x04, 0x02, 0xee, 0x07, 0x02, 0xee, 0x08,
// 									0x02, 0xee, 0x0b, 0x02, 0xee, 0x0c, 0x02, 0xee,
// 									0x0f, 0x02, 0xa0, 0x00, 0x4c, 0x02, 0x06};
// 	load_instructions(instructions);
// 	write_16bit(0xFFFC, 0x600);
// 	run();
// }
// void *snake(void *arg)
// {
// 	vector<uint8_t> instructions = {0x20, 0x06, 0x06, 0x20, 0x38, 0x06, 0x20, 0x0d, 0x06, 0x20, 0x2a, 0x06, 0x60, 0xa9, 0x02, 0x85,
// 									0x02, 0xa9, 0x04, 0x85, 0x03, 0xa9, 0x11, 0x85, 0x10, 0xa9, 0x10, 0x85, 0x12, 0xa9, 0x0f, 0x85,
// 									0x14, 0xa9, 0x04, 0x85, 0x11, 0x85, 0x13, 0x85, 0x15, 0x60, 0xa5, 0xfe, 0x85, 0x00, 0xa5, 0xfe,
// 									0x29, 0x03, 0x18, 0x69, 0x02, 0x85, 0x01, 0x60, 0x20, 0x4d, 0x06, 0x20, 0x8d, 0x06, 0x20, 0xc3,
// 									0x06, 0x20, 0x19, 0x07, 0x20, 0x20, 0x07, 0x20, 0x2d, 0x07, 0x4c, 0x38, 0x06, 0xa5, 0xff, 0xc9,
// 									0x77, 0xf0, 0x0d, 0xc9, 0x64, 0xf0, 0x14, 0xc9, 0x73, 0xf0, 0x1b, 0xc9, 0x61, 0xf0, 0x22, 0x60,
// 									0xa9, 0x04, 0x24, 0x02, 0xd0, 0x26, 0xa9, 0x01, 0x85, 0x02, 0x60, 0xa9, 0x08, 0x24, 0x02, 0xd0,
// 									0x1b, 0xa9, 0x02, 0x85, 0x02, 0x60, 0xa9, 0x01, 0x24, 0x02, 0xd0, 0x10, 0xa9, 0x04, 0x85, 0x02,
// 									0x60, 0xa9, 0x02, 0x24, 0x02, 0xd0, 0x05, 0xa9, 0x08, 0x85, 0x02, 0x60, 0x60, 0x20, 0x94, 0x06,
// 									0x20, 0xa8, 0x06, 0x60, 0xa5, 0x00, 0xc5, 0x10, 0xd0, 0x0d, 0xa5, 0x01, 0xc5, 0x11, 0xd0, 0x07,
// 									0xe6, 0x03, 0xe6, 0x03, 0x20, 0x2a, 0x06, 0x60, 0xa2, 0x02, 0xb5, 0x10, 0xc5, 0x10, 0xd0, 0x06,
// 									0xb5, 0x11, 0xc5, 0x11, 0xf0, 0x09, 0xe8, 0xe8, 0xe4, 0x03, 0xf0, 0x06, 0x4c, 0xaa, 0x06, 0x4c,
// 									0x35, 0x07, 0x60, 0xa6, 0x03, 0xca, 0x8a, 0xb5, 0x10, 0x95, 0x12, 0xca, 0x10, 0xf9, 0xa5, 0x02,
// 									0x4a, 0xb0, 0x09, 0x4a, 0xb0, 0x19, 0x4a, 0xb0, 0x1f, 0x4a, 0xb0, 0x2f, 0xa5, 0x10, 0x38, 0xe9,
// 									0x20, 0x85, 0x10, 0x90, 0x01, 0x60, 0xc6, 0x11, 0xa9, 0x01, 0xc5, 0x11, 0xf0, 0x28, 0x60, 0xe6,
// 									0x10, 0xa9, 0x1f, 0x24, 0x10, 0xf0, 0x1f, 0x60, 0xa5, 0x10, 0x18, 0x69, 0x20, 0x85, 0x10, 0xb0,
// 									0x01, 0x60, 0xe6, 0x11, 0xa9, 0x06, 0xc5, 0x11, 0xf0, 0x0c, 0x60, 0xc6, 0x10, 0xa5, 0x10, 0x29,
// 									0x1f, 0xc9, 0x1f, 0xf0, 0x01, 0x60, 0x4c, 0x35, 0x07, 0xa0, 0x00, 0xa5, 0xfe, 0x91, 0x00, 0x60,
// 									0xa6, 0x03, 0xa9, 0x00, 0x81, 0x10, 0xa2, 0x00, 0xa9, 0x01, 0x81, 0x10, 0x60, 0xa2, 0x00, 0xea,
// 									0xea, 0xca, 0xd0, 0xfb, 0x60};
// 	load_instructions(instructions);
// 	write_16bit(0xFFFC, 0x600);
// 	run();
// }
// void *pixelTest(void *arg)
// {
// 	vector<uint8_t> instructions = {
// 		0xa2, 0x00, 0xa0, 0x00, 0x8a, 0x99, 0x00, 0x02,	 // m
// 		0x48, 0xe8, 0xc8, 0xc0, 0x10, 0xd0, 0xf5, 0x68,	 // m
// 		0x99, 0x00, 0x02, 0xc8, 0xc0, 0x20, 0xd0, 0xf7}; // s
// 	load_instructions(instructions);
// 	write_16bit(0xFFFC, 0x600);
// 	run();
// }
// void *keyboard_test(void *arg)
// {
// 	vector<uint8_t> instructions = {0xa5, 0xff, 0x4C, 0x00, 0x06};
// 	load_instructions(instructions);
// 	write_16bit(0xFFFC, 0x600);
// 	run();
// }
// void *movement(void *arg)
// {
// 	vector<uint8_t> instructions = {0x20, 0x54, 0x06, 0x20, 0x70, 0x06, 0x20, 0xc9, 0x06, 0x4c, 0x03, 0x06, 0x60,
// 									0x48, 0x8a, 0x48, 0xa9, 0x00, 0xa6, 0x10, 0x9d, 0x00, 0x05, 0xa6, 0x78, 0xa9, 0x01,
// 									0x9d, 0x00, 0x05, 0x86, 0x10, 0xa9, 0x00, 0xa6, 0x11, 0x9d, 0x00, 0x05, 0xa6, 0x79,
// 									0xa9, 0x03, 0x9d, 0x00, 0x05, 0x86, 0x11, 0xa9, 0x00, 0xa6, 0x12, 0x9d, 0x00, 0x05,
// 									0xa6, 0x7a, 0xa9, 0x04, 0x9d, 0x00, 0x05, 0x86, 0x12, 0xa9, 0x00, 0xa6, 0x13, 0x9d,
// 									0x00, 0x05, 0xa6, 0x7b, 0xa9, 0x04, 0x9d, 0x00, 0x05, 0x86, 0x13, 0x68, 0xaa, 0x68,
// 									0x60, 0xa2, 0x00, 0xad, 0x0a, 0x07, 0x9d, 0x00, 0x02, 0x9d, 0x00, 0x04, 0xca, 0xe0,
// 									0x00, 0xd0, 0xf5, 0xa9, 0x10, 0x85, 0x80, 0xa2, 0x0f, 0x95, 0x81, 0xca, 0x10, 0xfb,
// 									0x60, 0xa9, 0x00, 0x85, 0x78, 0xa9, 0x20, 0x85, 0x79, 0xa9, 0xc0, 0x85, 0x7a, 0xa9,
// 									0xe0, 0x85, 0x7b, 0xa2, 0x0f, 0xb5, 0x81, 0x95, 0x82, 0xa8, 0x84, 0x02, 0xb9, 0xea,
// 									0x06, 0x85, 0x00, 0xc8, 0xb9, 0xea, 0x06, 0x85, 0x01, 0xad, 0x0a, 0x07, 0xa4, 0x78,
// 									0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x7b, 0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x79,
// 									0xa9, 0x00, 0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x7a, 0x91, 0x00, 0xc8, 0x91, 0x00,
// 									0xe6, 0x78, 0xe6, 0x79, 0xe6, 0x7a, 0xe6, 0x7b, 0xe6, 0x78, 0xe6, 0x79, 0xe6, 0x7a,
// 									0xe6, 0x7b, 0xca, 0x10, 0xba, 0x60, 0xa5, 0x80, 0xc5, 0x81, 0xf0, 0x09, 0xa5, 0x80,
// 									0x18, 0xe5, 0x81, 0x10, 0x0f, 0x30, 0x08, 0xa5, 0xfe, 0x29, 0x0f, 0x0a, 0x85, 0x80,
// 									0x60, 0xc6, 0x81, 0xc6, 0x81, 0x60, 0xe6, 0x81, 0xe6, 0x81, 0x60, 0x00, 0x02, 0x20,
// 									0x02, 0x40, 0x02, 0x60, 0x02, 0x80, 0x02, 0xa0, 0x02, 0xc0, 0x02, 0xe0, 0x02, 0x00,
// 									0x03, 0x20, 0x03, 0x40, 0x03, 0x60, 0x03, 0x80, 0x03, 0xa0, 0x03, 0xc0, 0x03, 0xe0,
// 									0x03, 0x0d};
// 	// vector<uint8_t> instructions = {0x48, 0x8A, 0x48, 0xA9, 0x00, 0xA6, 0x10, 0x9D, 0x00, 0x05, 0xA6, 0x78, 0xA9, 0x01, 0x9D, 0x00,
// 	// 								0x05, 0x86, 0x10, 0xA9, 0x00, 0xA6, 0x11, 0x9D, 0x00, 0x05, 0xA6, 0x79, 0xA9, 0x03, 0x9D, 0x00,
// 	// 								0x05, 0x86, 0x11, 0xA9, 0x00, 0xA6, 0x12, 0x9D, 0x00, 0x05, 0xA6, 0x7A, 0xA9, 0x04, 0x9D, 0x00,
// 	// 								0x05, 0x86, 0x12, 0xA9, 0x00, 0xA6, 0x13, 0x9D, 0x00, 0x05, 0xA6, 0x7B, 0xA9, 0x04, 0x9D, 0x00,
// 	// 								0x05, 0x86, 0x13, 0x68, 0xAA, 0x68, 0xe6, 0x7a, 0x4c, 0x00, 0x06};
// 	load_instructions(instructions);
// 	write_16bit(0xFFFC, 0x600);
// 	run();
// }

void threadFunction(int id)
{
	// while (1)
	// cout << "Thread " << id << " is running\n";
	// vector<uint8_t> instructions = {
	// 	0xa2, 0x00, 0xa0, 0x00, 0x8a, 0x99, 0x00, 0x02,	 // m
	// 	0x48, 0xe8, 0xc8, 0xc0, 0x10, 0xd0, 0xf5, 0x68,	 // m
	// 	0x99, 0x00, 0x02, 0xc8, 0xc0, 0x20, 0xd0, 0xf7}; // s
	// vector<uint8_t> instructions = {0x20, 0x06, 0x06, 0x20, 0x38, 0x06, 0x20, 0x0d, 0x06, 0x20, 0x2a, 0x06, 0x60, 0xa9, 0x02, 0x85,
	// 								0x02, 0xa9, 0x04, 0x85, 0x03, 0xa9, 0x11, 0x85, 0x10, 0xa9, 0x10, 0x85, 0x12, 0xa9, 0x0f, 0x85,
	// 								0x14, 0xa9, 0x04, 0x85, 0x11, 0x85, 0x13, 0x85, 0x15, 0x60, 0xa5, 0xfe, 0x85, 0x00, 0xa5, 0xfe,
	// 								0x29, 0x03, 0x18, 0x69, 0x02, 0x85, 0x01, 0x60, 0x20, 0x4d, 0x06, 0x20, 0x8d, 0x06, 0x20, 0xc3,
	// 								0x06, 0x20, 0x19, 0x07, 0x20, 0x20, 0x07, 0x20, 0x2d, 0x07, 0x4c, 0x38, 0x06, 0xa5, 0xff, 0xc9,
	// 								0x77, 0xf0, 0x0d, 0xc9, 0x64, 0xf0, 0x14, 0xc9, 0x73, 0xf0, 0x1b, 0xc9, 0x61, 0xf0, 0x22, 0x60,
	// 								0xa9, 0x04, 0x24, 0x02, 0xd0, 0x26, 0xa9, 0x01, 0x85, 0x02, 0x60, 0xa9, 0x08, 0x24, 0x02, 0xd0,
	// 								0x1b, 0xa9, 0x02, 0x85, 0x02, 0x60, 0xa9, 0x01, 0x24, 0x02, 0xd0, 0x10, 0xa9, 0x04, 0x85, 0x02,
	// 								0x60, 0xa9, 0x02, 0x24, 0x02, 0xd0, 0x05, 0xa9, 0x08, 0x85, 0x02, 0x60, 0x60, 0x20, 0x94, 0x06,
	// 								0x20, 0xa8, 0x06, 0x60, 0xa5, 0x00, 0xc5, 0x10, 0xd0, 0x0d, 0xa5, 0x01, 0xc5, 0x11, 0xd0, 0x07,
	// 								0xe6, 0x03, 0xe6, 0x03, 0x20, 0x2a, 0x06, 0x60, 0xa2, 0x02, 0xb5, 0x10, 0xc5, 0x10, 0xd0, 0x06,
	// 								0xb5, 0x11, 0xc5, 0x11, 0xf0, 0x09, 0xe8, 0xe8, 0xe4, 0x03, 0xf0, 0x06, 0x4c, 0xaa, 0x06, 0x4c,
	// 								0x35, 0x07, 0x60, 0xa6, 0x03, 0xca, 0x8a, 0xb5, 0x10, 0x95, 0x12, 0xca, 0x10, 0xf9, 0xa5, 0x02,
	// 								0x4a, 0xb0, 0x09, 0x4a, 0xb0, 0x19, 0x4a, 0xb0, 0x1f, 0x4a, 0xb0, 0x2f, 0xa5, 0x10, 0x38, 0xe9,
	// 								0x20, 0x85, 0x10, 0x90, 0x01, 0x60, 0xc6, 0x11, 0xa9, 0x01, 0xc5, 0x11, 0xf0, 0x28, 0x60, 0xe6,
	// 								0x10, 0xa9, 0x1f, 0x24, 0x10, 0xf0, 0x1f, 0x60, 0xa5, 0x10, 0x18, 0x69, 0x20, 0x85, 0x10, 0xb0,
	// 								0x01, 0x60, 0xe6, 0x11, 0xa9, 0x06, 0xc5, 0x11, 0xf0, 0x0c, 0x60, 0xc6, 0x10, 0xa5, 0x10, 0x29,
	// 								0x1f, 0xc9, 0x1f, 0xf0, 0x01, 0x60, 0x4c, 0x35, 0x07, 0xa0, 0x00, 0xa5, 0xfe, 0x91, 0x00, 0x60,
	// 								0xa6, 0x03, 0xa9, 0x00, 0x81, 0x10, 0xa2, 0x00, 0xa9, 0x01, 0x81, 0x10, 0x60, 0xa2, 0x00, 0xea,
	// 								0xea, 0xca, 0xd0, 0xfb, 0x60};

	// load_instructions(instructions);
	// write_16bit(0xFFFC, 0x0600);
	// run();
}
struct A
{
	int a, b;
};
int main(int argc, char *argv[])
{
	// const int8_t *c = "hello world";
	// signed char* c = "hello world";
	// printf("%s \n", c);
	// Rom rom = modify_for_NESfile(load_rom(argv[1]));
	init(argv[1]);
	// A a = {1, 2};
	// load_instructions(rom.PRG);
	// write_16bit(0xFFFC, 0x8000);
	// run();
	// runsfml();
	// unsigned char char char a = 16777215;
	// uint32_t value : 24;
	// value = 16777217;
	// int some_int;
	// auto x = NAME_OF(some_int);
	// Object obj(42);
	// (void)some_int;
	// cout << x << endl;
	// snake();
	// vector<uint8_t> instructions = {0xE6, 0x80};
	// printf()
	// load_instructions(instructions);
	//
	// run();
	// printf("memory: %x \n", read_8bit(0x0));

	// load_instructions({0xE6, 0x80});
	// write_16bit(0xFFFC, 0x600);
	// run();
	// printf("%x \n", read_8bit(0x80));
	// Bus b;
	// b.helloWorld();
	// pthread_t myThread;
	// thread t1(threadFunction, 1);
	// t1.detach();
	// runsfml();

	return 1;
}