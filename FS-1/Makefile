CXX = g++
CXXFLAGS = -Wall -std=c++11

all: read-file

read-file: read-file.o
	$(CXX) $(CXXFLAGS) -o read-file read-file.o

read-file.o: read-file.cpp
	$(CXX) $(CXXFLAGS) -c read-file.cpp

clean:
	rm -f *.o read-file

.PHONY: all clean

