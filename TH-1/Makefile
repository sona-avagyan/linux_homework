CXX = g++
CXXFLAGS = -Wall -std=c++11


array_summary: array_summary.o
	$(CXX) $(CXXFLAGS) -o array_summary array_summary.o

array_summary.o: array_summary.cpp
	$(CXX) $(CXXFLAGS) -c array_summary.cpp

clean:
	rm -f *.o array_summary
