CXX = g++
CXXFLAGS = -Wall -std=c++11

all: redirect-cin

redirect-cin: redirect-cin.o
	$(CXX) $(CXXFLAGS) -o redirect-cin redirect-cin.o

redirect-cin.o: redirect-cin.cpp
	$(CXX) $(CXXFLAGS) -c redirect-cin.cpp

clean:
	rm -f *.o redirect-cin

.PHONY: all clean
