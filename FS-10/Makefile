CXX = g++
CXXFLAGS = -Wall -std=c++11

all: copy

copy: copy.o
	$(CXX) $(CXXFLAGS) -o copy copy.o

copy.o: copy.cpp
	$(CXX) $(CXXFLAGS) -c copy.cpp

clean:
	rm -f *.o copy

.PHONY: all clean
