CXX = g++
CXXFLAGS = -Wall -std=c++11

all:manual-append

manual-append: manual-append.o
	$(CXX) $(CXXFLAGS) -o manual-append manual-append.o

manual-append.o: manual-append.cpp
	$(CXX) $(CXXFLAGS) -c manual-append.cpp

clean:
	rm -f *.o manual-append

.PHONY: all clean
