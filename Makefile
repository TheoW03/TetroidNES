all: main.cpp BitOperations.cpp Memory.cpp
	g++ BitOperations.o Memory.o main.o -L SDL2/lib -Wall -lSDL2main -lSDL2 -o test.exe
BitOperations.cpp: 
	g++ -c src/emulator/BitOperations.cpp -I SDL2/include -L SDL2/lib
Memory.cpp: 
	g++ -c src/emulator/Memory.cpp -I SDL2/include -L SDL2/lib
main.cpp: src/emulator/BitOperations.h src/emulator/Memory.h
	g++ -c src/main.cpp -I SDL2/include -L SDL2/lib

