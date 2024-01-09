all: main.cpp StatusRegister.cpp Computer.cpp BitOperations.cpp Instructions.cpp Memory.cpp AddressMode.cpp
	g++ StatusRegister.o AddressMode.o BitOperations.o Instructions.o Computer.o Memory.o main.o -L SDL2/lib -Wall -lSDL2main -lSDL2 -o test.exe
BitOperations.cpp: src/emulator/BitOperations.h 
	g++ -c src/emulator/BitOperations.cpp -I SDL2/include -L SDL2/lib
Memory.cpp: src/emulator/Memory.h
	g++ -c src/emulator/Memory.cpp -I SDL2/include -L SDL2/lib
Computer.cpp: src/emulator/BitOperations.h src/emulator/Memory.h src/emulator/Computer.h src/emulator/Instructions.h  
	g++ -c src/emulator/Computer.cpp -I SDL2/include -L SDL2/lib
main.cpp: src/emulator/BitOperations.h src/emulator/Memory.h src/emulator/Computer.h 
	g++ -c src/main.cpp -I SDL2/include -L SDL2/lib
Instructions.cpp: src/emulator/Instructions.h  src/emulator/Memory.h 
	g++ -c src/emulator/Instructions.cpp -I SDL2/include -L SDL2/lib
AddressMode.cpp: src/emulator/AddressMode.h  src/emulator/Memory.h 
	g++ -c src/emulator/AddressMode.cpp -I SDL2/include -L SDL2/lib
StatusRegister.cpp: src/emulator/StatusRegister.h src/emulator/Computer.h
	g++ -c src/emulator/StatusRegister.cpp -I SDL2/include -L SDL2/lib
clean:
	rm *.o *.exe
	clear
	
