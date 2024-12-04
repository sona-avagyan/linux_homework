CXX = g++
CXXFLAGS = -Wall -std=c++11


signal-echo: signal-echo.o
	$(CXX) $(CXXFLAGS) -o signal-echo signal-echo.o

signal-echo.o: signal-echo.cpp
	$(CXX) $(CXXFLAGS) -c signal-echo.cpp

clean:
	rm -f *.o signal-echo
