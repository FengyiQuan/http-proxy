//
//  proxy.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"
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

Proxy::Proxy()
{
}

Proxy::Proxy(size_t port) : portNum(port)
{
}

// TODO: close socket
Proxy::~Proxy()
{
}

int Proxy::run()
{
    if (initSocketClient() < 0)
    {
        // LOG
        return -1;
    }

    while (true)
    {
        // accept client connection
        socklen_t cliLen = sizeof(clientAddr);
        int new_socket = accept(client_fd, (struct sockaddr *)&clientAddr, &cliLen);

        if (new_socket < 0)
        {
            // LOG
            return -1;
        }

        // pthread_t thread;
        // if (fork() == 0)
        // {
        if (handleRequest())
        {
            return -1;
        }
        return 0;
        // }
        close(new_socket);
    }

    return 0;
}

int Proxy::initSocketClient()
{
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        // LOG
        return -1;
    }
    proxyAddr.sin_family = AF_INET;
    proxyAddr.sin_addr.s_addr = INADDR_ANY;
    proxyAddr.sin_port = htons(portNum);

    // bind to port 12345
    int bindResult = bind(client_fd, (struct sockaddr *)&proxyAddr, sizeof(proxyAddr));
    if (bindResult < 0)
    {
        // LOG
        return -1;
    }
    // listen for client socket
    if (listen(client_fd, 5) < 0)
    {
        //         perror("listen");
        // LOG
        return -1;
    }

    return 0;
}

int Proxy::handleRequest()
{
    //    httpClientRequest = new HTTPRequest();
    //    httpClientResponse= new HTTPResponse();
    //    httpServerRequest = new HTTPRequest();
    //    httpServerResponse = new HTTPResponse();
    // std::cout << "hello world" << std::endl;

    int buf_sz = 65536;
    char *buf = new char[buf_sz];
    memset(buf, '\0', buf_sz);
    int recvLength = recv(client_fd, buf, buf_sz, 0);
    if (recvLength < 0)
    {
        // LOG
        return -1;
    }
    return 0;
}

int Proxy::handleConnect(void)
{
}
int handleGet(void);
int handlePost(void);

// int Proxy::initSocketServer(void);

// int Proxy::recvRequestClient(void);
// int Proxy::parseRequestClient(void);
// int Proxy::processRequestClient(void);
// int Proxy::prepareRequestServer(void);
// int Proxy::sendRequestServer(void);
// int Proxy::recvResponseServer(void);
// int Proxy::parseResponseServer(void);
// int Proxy::processResponseServer(void);
// int Proxy::prepareResponseClient(void);
// int Proxy::sendResponseClient(void);