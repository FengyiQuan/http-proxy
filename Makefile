CFLAGS = -Wall -Werror -std=gnu++11 -pedantic -ggdb3
#  

all: proxyyy

proxyyy: main.cpp main.hpp proxy.hpp request.hpp response.hpp parser.hpp logger.hpp socket.hpp threadObject.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp request.cpp response.cpp parser.cpp logger.cpp socket.cpp threadObject.cpp -lpthread

clean:
	rm -rf *.o proxyyy

# vcm-30870.vm.duke.edu
