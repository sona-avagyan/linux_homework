CXX = g++
CXXFLAGS = -Wall -std=c++11

all: do-command

do-command: do-command.o
	$(CXX) $(CXXFLAGS) -o do-command do-command.o

do-command.o: do-command.cpp
	$(CXX) $(CXXFLAGS) -c do-command.cpp

clean:
	rm -f *.o do-command

.PHONY: all clean
