CXX = g++
CXXFLAGS = -Wall -std=c++11

all: shell

shell: shell.o
	$(CXX) $(CXXFLAGS) -o shell shell.o

shell.o: shell.cpp
	$(CXX) $(CXXFLAGS) -c shell.cpp

clean:
	rm -f *.o shell

