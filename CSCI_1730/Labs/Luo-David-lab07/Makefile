all: lab07

lab07: lab07.o
	g++ -Wall -g -lncurses -o lab07 lab07.o

lab07.o: lab07.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -lncurses -c lab07.cpp	
clean:
	rm -f lab07 lab07.o 
