.PHONY: all clean
		
all: ledserver

clean:
	rm -rf ledserver *.o

drivers.o: drivers.cpp drivers.hpp
	g++ -std=c++11 -c drivers.cpp

handlers.o: handlers.cpp handlers.hpp drivers.hpp
	g++ -std=c++11 -c handlers.cpp

interfaces.o: interfaces.cpp interfaces.hpp handlers.hpp drivers.hpp
	g++ -pthread -std=c++11 -c interfaces.cpp

ledserver.o: ledserver.cpp interfaces.hpp handlers.hpp drivers.hpp
	g++ -std=c++11 -c ledserver.cpp

ledserver: drivers.o interfaces.o handlers.o ledserver.o
	g++ -pthread -std=c++11 drivers.o handlers.o interfaces.o ledserver.o -o ledserver

install: ledserver
	rm -rf *.o
