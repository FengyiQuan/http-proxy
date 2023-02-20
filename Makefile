CFLAGS = -ggdb3
#  -Wall -Werror -std=gnu++98 -pedantic 

all: proxyyy

proxyyy: main.cpp main.hpp
	g++ $(CFLAGS) -o proxyyy main.cpp proxy.cpp request.cpp

clean:
	rm -rf *.o proxyyy

# nc localhost 12345
# GET /wx/in/kanpur/wx.php HTTP/1.0 \r\n Host: www.rssweather.com \r\n \r\n