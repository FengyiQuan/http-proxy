all: main

main: main.cpp proxy.cpp proxy.hpp
	g++ -ggdb3 main.cpp proxy.cpp

clean:
	rm -rf *.o main
