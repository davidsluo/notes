all: stat

stat: stat.o
	g++ -o stat stat.o -Wall -std=c++14 -g -O0 -pedantic-errors

stat.o: stat.cpp
	g++ -c stat.cpp

minmax: minmax.o
	g++ -o minmax minmax.o -Wall -std=c++14 -g -O0 -pedantic-errors

minmax.o: minmax.cpp
	g++ -c minmax.cpp

clean:
	rm -f stat
	rm -f minmax
	rm -f *.o
