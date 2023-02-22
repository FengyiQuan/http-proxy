//
//  proxy.hpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#ifndef proxy_hpp
#define proxy_hpp

#include <stdio.h>
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

#include "request.hpp"
class Proxy
{
public:
    Proxy();
    Proxy(size_t port);
    ~Proxy();
    int run(void);

private:
    size_t portNum;

    int initSocketClient(void);
    int initSocketServer(std::string address, size_t port);

    int handleRequest(void);
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
    int handleConnect(Request *request);
    int handleGet(Request *request);
    int handlePost(Request *request);

    int client_fd, client_fd_connection, server_fd;
    // size_t serverPort;
    struct sockaddr_in serverAddr, clientAddr;
};

#endif /* proxy_hpp */
