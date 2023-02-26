//
//  proxy.hpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#ifndef proxy_hpp
#define proxy_hpp

#include <stdio.h>
#include <ctime>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <windows.h>
#include <vector>
#include <errno.h>

#include "request.hpp"
#include "response.hpp"
#include "socket.hpp"
#include "logger.hpp"
#include "cache.hpp"
// #include "threadObject.hpp"
#define BUF_LEN 65536

// #define OK "HTTP/1.1 200 OK\r\n\r\n"
// #define BADREQUEST "HTTP/1.1 400 Bad Request\r\n\r\n"
// #define BADGATEWAY "HTTP/1.1 502 Bad Gateway\r\n\r\n"
// #define NOTIMPLEMENTED "HTTP/1.1 501 Not Implemented\r\n\r\n"
const std::string ok = "HTTP/1.1 200 OK\r\n\r\n";
const std::string badRequest = "HTTP/1.1 400 Bad Request\r\n\r\n";
const std::string badGateway = "HTTP/1.1 502 Bad Gateway\r\n\r\n";
const std::string notImplemented = "HTTP/1.1 501 Not Implemented\r\n\r\n";

const std::vector<char> OK(ok.begin(), ok.end());
const std::vector<char> BADREQUEST(badRequest.begin(), badRequest.end());
const std::vector<char> BADGATEWAY(badGateway.begin(), badGateway.end());
const std::vector<char> NOTIMPLEMENTED(notImplemented.begin(), notImplemented.end());

class Proxy
{
public:
    Proxy(size_t port);
    // Proxy(std::string port);
    ~Proxy();
    int run(void);

private:
    size_t portNum;
    // std::string portNum;

    int initSocketServer(void);
    int initSocketClient(std::string address, size_t port);

    // int handleRequest(ThreadObject *threadObject);
    int handleRequest(int client_fd_connection, std::string ip, size_t requestId, Cache *cache);
    // int recvRequestClient(void);
    // int parseRequestClient(void);
    // int processRequestClient(void);
    // int prepareRequestServer(void);
    // int sendRequestServer(void);
    // int recvResponseServer(void);
    // int parseResponseServer(void);
    // int processResponseServer(void);
    // int prepareResponseClient(void);
    // int sendResponseClient(void);
    int handleConnect(Request *request, int client_fd_connection, int requestId);
    int handleGet(Request *request, int client_fd_connection, int requestId);
    int handlePost(Request *request, int client_fd_connection, int requestId);
    bool revalidate(Request *request, int server_fd, int requestId, Cache *cache);

    int client_fd;
    //   server_fd;
    // size_t serverPort;
    struct sockaddr_in clientAddr;
    // serverAddr,

    // helper to_string for not using c++ 11
    template <class T>
    std::string my_to_string(const T &value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
};
#endif /* proxy_hpp */
