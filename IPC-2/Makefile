CXX = g++
CXXFLAGS = -Wall -std=c++17


prime_calculator: prime_calculator.o
	$(CXX) $(CXXFLAGS) -o prime_calculator prime_calculator.o

prime_calculator.o: prime_calculator.cpp
	$(CXX) $(CXXFLAGS) -c prime_calculator.cpp

clean:
	rm -f *.o prime_calculator
