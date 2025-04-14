all: init client deinit
 
init: init.o bank.o
	g++ init.o bank.o -o init -g

client: client.o bank.o
	g++ client.o bank.o  -o client -g

client.o: client.cpp
	g++ client.cpp -c -o client.o -g

init.o: init.cpp bank.h
	g++ init.cpp -c -o init.o -g

bank.o: bank.cpp bank.h
	g++ bank.cpp -c -o bank.o -g

deinit: deinit.o 
	g++ deinit.o -o deinit

deinit.o: deinit.cpp
	g++ deinit.cpp -c -o deinit.o

clean:
	rm -rf *.o init client deinit
