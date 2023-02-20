CFLAGS = -ggdb3
#  -Wall -Werror -std=gnu++98 -pedantic 

all: proxyyy

proxyyy: main.cpp main.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp request.cpp

clean:
	rm -rf *.o proxyyy

