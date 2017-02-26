CC = g++
DEBUG = -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: p1

p1: p1.o Matrix.o
	$(CC) $(LFLAGS) -o p1 p1.o Matrix.o

p1.o: p1.cpp Matrix.h
	$(CC) $(CFLAGS) p1.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(CFLAGS) Matrix.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf p1
	
