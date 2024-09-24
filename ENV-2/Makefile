iCXX=g++
CXXFLAGS=-Wall -std=c++11

all: program

program: main.o complex.o sorting.o
	$(CXX) $(CXXFLAGS) -o program main.o complex.o sorting.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Complex.o: complex.cpp complex.hpp
	$(CXX) $(CXXFLAGS) -c Complex.cpp

Sort.o: sorting.cpp sorting.hpp
	$(CXX) $(CXXFLAGS) -c sorting.cpp

clean:
	rm -f *.o program

.PHONY: all clean

