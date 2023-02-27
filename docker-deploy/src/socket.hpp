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
#include <limits>
#include <climits>
#include <vector>

// create a server socket and return the socket file descriptor
int init_server(std::string port);
// connect to a server and return the socket file descriptor
int init_client(std::string hostname, std::string port);
// listen for a connection and return the socket file descriptor
int server_accept(int socket_fd, std::string *ip);

// send data to a socket
void send_data(int socket_fd, std::vector<char> data, size_t data_len);
// receive data from a socket
int recv_data(int socket_fd, std::vector<char> &req_msg);

#endif /* socket_hpp */
