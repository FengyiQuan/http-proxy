all: main

main: main.cpp proxy.cpp proxy.h
	g++ -o main main.cpp proxy.cpp

clean:
	rm -rf *.o main
