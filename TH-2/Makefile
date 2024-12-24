CXX = g++
CXXFLAGS = -Wall -std=c++17

parallel_scheduler.so: parallel_scheduler.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

demo_application: demo_application.o parallel_scheduler.so
	$(CXX) $(CXXFLAGS) -o demo_application demo_application.o -L. -lparallel_scheduler -lpthread

demo_application.o: demo_application.cpp
	$(CXX) $(CXXFLAGS) -c demo_application.cpp

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.hpp
	$(CXX) $(CXXFLAGS) -fPIC -c parallel_scheduler.cpp

clean:
	rm -f *.o demo_application parallel_scheduler.so

.PHONY: all clean
