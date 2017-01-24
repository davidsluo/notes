all: shifty
	./shifty

shifty: shifty.o
	g++ -o shifty shifty.o

shifty.o: shifty.cpp
	g++ -c shifty.cpp

clean:
	rm -f shifty
	rm -f *.o
