CXX = g++
CXXFLAGS = -Wall -std=c++11

all: simple-copy

simple-copy: simple-copy.o
	$(CXX) $(CXXFLAGS) -o simple-copy simple-copy.o

simple-copy.o: simple-copy.cpp
	$(CXX) $(CXXFLAGS) -c simple-copy.cpp

clean:
	rm -f *.o simple-copy

.PHONY: all clean

