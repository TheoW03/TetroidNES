all: main.cpp
	g++ main.o -L SDL2/lib -Wall -lSDL2main -lSDL2 -o test.exe
main.cpp:
	g++ -c src/main.cpp -I SDL2/include -L SDL2/lib
