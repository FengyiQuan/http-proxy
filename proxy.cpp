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

        if (fork() == 0)
        {
            if (handleRequest())
            {
                return -1;
            }
            return 0;
        }
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
    std::cout << "hello world" << std::endl;
    return 0;
}
