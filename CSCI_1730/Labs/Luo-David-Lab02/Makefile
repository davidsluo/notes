all: stat

stat: stat.o
	g++ -o stat stat.o -Wall -std=c++14 -g -O0 -pedantic-errors

stat.o: stat.cpp
	g++ -c stat.cpp

clean:
	rm -f stat
	rm -f *.o
