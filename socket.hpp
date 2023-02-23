#ifndef socket_hpp
#define socket_hpp

#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>

// create a server socket and return the socket file descriptor
int init_server(std::string port);
// connect to a server and return the socket file descriptor
int init_client(std::string hostname, std::string port);
// listen for a connection and return the socket file descriptor
int server_accept(int socket_fd, std::string *ip);

#endif /* socket_hpp */
