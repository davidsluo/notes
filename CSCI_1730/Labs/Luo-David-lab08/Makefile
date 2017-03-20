TARGET=cat

all: $(TARGET)

$(TARGET): $(TARGET).o
	g++ -Wall -g -o $(TARGET) $(TARGET).o 

$(TARGET).o: $(TARGET).cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c $(TARGET).cpp

clean:
	rm -f $(TARGET) $(TARGET).o
