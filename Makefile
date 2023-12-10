all: main.cpp Computer.cpp BitOperations.cpp Memory.cpp
	g++ BitOperations.o Computer.o Memory.o main.o -L SDL2/lib -Wall -lSDL2main -lSDL2 -o test.exe
BitOperations.cpp: src/emulator/BitOperations.h 
	g++ -c src/emulator/BitOperations.cpp -I SDL2/include -L SDL2/lib
Memory.cpp: src/emulator/Memory.h
	g++ -c src/emulator/Memory.cpp -I SDL2/include -L SDL2/lib
Computer.cpp: src/emulator/BitOperations.h src/emulator/Memory.h src/emulator/Computer.h
	g++ -c src/emulator/Computer.cpp -I SDL2/include -L SDL2/lib
main.cpp: src/emulator/BitOperations.h src/emulator/Memory.h src/emulator/Computer.h 
	g++ -c src/main.cpp -I SDL2/include -L SDL2/lib
clean:
	rm *.o *.exe
