CFLAGS = -Wall -Werror -std=gnu++98 -pedantic -ggdb3
#  

all: proxyyy

proxyyy: main.cpp main.hpp proxy.hpp request.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp request.cpp

clean:
	rm -rf *.o proxyyy

# nc localhost 12345
# GET /wx/in/kanpur/wx.php HTTP/1.0 \r\nHost: www.rssweather.com \r\n\r\n