CXX = g++
CXXFLAGS = -Wall -std=c++11

all: hacker-rm

hacker-rm: hacker-rm.o
	$(CXX) $(CXXFLAGS) -o hacker-rm hacker-rm.o

hacker-rm.o: hacker-rm.cpp
	$(CXX) $(CXXFLAGS) -c hacker-rm.cpp

clean:
	rm -f *.o hacker-rm

.PHONY: all clean
