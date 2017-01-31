SOURCE = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
NAMES = $(basename $(SOURCE))

all: $(NAMES)

%: %.o
	g++ -o $@ $< -Wall -std=c++14 -g -O0 -pedantic-errors

%.o: %.cpp
	g++ -c $<

clean:
	rm -f $(NAMES) 
