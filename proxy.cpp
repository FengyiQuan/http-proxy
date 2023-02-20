//
//  proxy.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"
#include "request.hpp"

Proxy::Proxy()
{
}

Proxy::Proxy(size_t port) : portNum(port)
{
}

// TODO: close socket
Proxy::~Proxy()
{
    close(client_fd);
    close(client_fd_connection);
    close(server_fd);
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
        client_fd_connection = accept(client_fd, (struct sockaddr *)&clientAddr, &cliLen);

        if (client_fd_connection < 0)
        {
            // LOG
            std::cerr << "Error accepting client connection" << std::endl;
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
        close(client_fd_connection);
    }

    return 0;
}

int Proxy::initSocketClient()
{
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        // LOG
        std::cerr << "Error creating client socket" << std::endl;
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

    //    httpClientResponse= new HTTPResponse();
    //    httpServerRequest = new HTTPRequest();
    //    httpServerResponse = new HTTPResponse();
    // std::cout << "hello world" << std::endl;

    // int buf_sz = 65536;
    // char *buf = new char[buf_sz];
    // memset(buf, '\0', buf_sz);
    char req_msg[65536] = {0};
    int recvLength = recv(client_fd, req_msg, sizeof(req_msg), 0); // fisrt request from client
    // std::cout << "len: " << len << std::endl;
    // int recvLength = recv(client_fd_connection, buf, buf_sz, 0); // fisrt request from client
    // std::cout << "recvLength: " << recvLength << std::endl;
    if (recvLength < 0)
    {
        // LOG
        std::cerr << "Error receiving client request" << std::endl;
        return -1;
    }
    // std::cout << "hello world" << std::endl;
    Request *httpClientRequest = new Request(req_msg);
    // print client request info
    // std::cout << "client request: " << std::endl;
    std::cout << "method: " << httpClientRequest->getMethod() << std::endl;
    std::cout << "request target: " << httpClientRequest->getRequestTarget() << std::endl;
    // // std::cout << "version: " << httpClientRequest->getHeaders() << std::endl;
    std::cout << "headers: " << std::endl;
    std::map<std::string, std::string> myMap = httpClientRequest->getHeaders();
    for (std::map<std::string, std::string>::iterator it = myMap.begin(); it != myMap.end(); ++it)
    {
        std::cout << it->first << ":" << it->second <<  "\n";
    }

    return 0;
}


// request: GET http://123.com/ HTTP/1.1
// Host: 123.com
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/110.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1

// int Proxy::handleConnect(void)
// {
//     return 0;
// }
// int handleGet(void);
// int handlePost(void);

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
