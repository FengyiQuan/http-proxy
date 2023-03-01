CFLAGS = -Wall -Werror -std=gnu++11 -pedantic -ggdb3
#  

all: proxyyy

daemon: daemon.cpp main.hpp proxy.hpp request.hpp response.hpp parser.hpp logger.hpp socket.hpp cache.hpp
	g++ $(CFLAGS) -o proxyyy daemon.cpp proxy.cpp request.cpp response.cpp parser.cpp logger.cpp socket.cpp cache.cpp -lpthread

proxyyy: main.cpp main.hpp proxy.hpp request.hpp response.hpp parser.hpp logger.hpp socket.hpp cache.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp request.cpp response.cpp parser.cpp logger.cpp socket.cpp cache.cpp -lpthread

clean:
	rm -rf *.o proxyyy

# vcm-30870.vm.duke.edu
