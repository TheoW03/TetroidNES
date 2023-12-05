all: main.cpp BitOperations.cpp
	g++ BitOperations.o main.o -L SDL2/lib -Wall -lSDL2main -lSDL2 -o test.exe
BitOperations.cpp: 
	g++ -c src/emulator/BitOperations.cpp -I SDL2/include -L SDL2/lib
main.cpp: src/emulator/BitOperations.h
	g++ -c src/main.cpp -I SDL2/include -L SDL2/lib

