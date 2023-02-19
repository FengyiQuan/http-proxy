CFLAGS = -Wall -Werror -std=gnu++98 -pedantic -ggdb3

all: proxyyy

proxyyy: main.cpp proxy.cpp proxy.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp

clean:
	rm -rf *.o proxyyy
