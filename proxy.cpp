//
//  proxy.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"


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
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(portNum);

    // bind to port 12345
    int bindResult = bind(client_fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
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

// socket conect to server
int Proxy::initSocketServer(std::string addr, size_t port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        // LOG
        std::cerr << "Error creating server socket" << std::endl;
        return -1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());

    if ((connect(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) < 0)
    {
        // cerr<<funcName<<"Failed to connect"<<endl;
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
    int recvLength = recv(client_fd_connection, req_msg, sizeof(req_msg), 0); // fisrt request from client
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
    if (httpClientRequest->getMethod() == "CONNECT")
    {
        handleConnect(httpClientRequest);
    }
    else if (httpClientRequest->getMethod() == "GET")
    {
        handleGet(httpClientRequest);
    }
    else if (httpClientRequest->getMethod() == "POST")
    {
        handlePost(httpClientRequest);
    }
    else
    {
        // LOG
        std::cerr << "Error: unknown method" << std::endl;
        std::string req400 = "HTTP/1.1 400 Bad Request";
        delete httpClientRequest;
        return -1;
    }
    delete httpClientRequest;
    return 0;
}

// handleConnect
int Proxy::handleConnect(Request *request)
{
    // make socket connection to server

    if (initSocketServer(request->getHost(), request->getPort()) < 0)
    {
        // LOG
        return -1;
    }

    return 0;
}

// handleGet from client and server
int Proxy::handleGet(Request *request)
{
    if (initSocketServer(request->getHost(), request->getPort()) < 0)
    {
        // LOG
        return -1;
    }

    std::string requestData = request->getData();
    size_t dataSize = requestData.size();

    if (send(server_fd, requestData.c_str(), dataSize, 0) < 0) // send request to server)
    {
        std::cerr << "Sending request failed" << std::endl;
    }

    char server_msg[65536] = {0};
    int mes_len = recv(server_fd, server_msg, sizeof(server_msg), 0); // receive response from server

    // print response
    //print mes_len
    std::cout << "Response from server: " << mes_len << std::endl;
    std::cout << "Response from server: " << server_msg << std::endl;
    // if ((send(sockfd_serv, buf, requestSize, 0)) < 0)

    // delete buf;
    return 0;
}

int Proxy::handlePost(Request *request)
{
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
