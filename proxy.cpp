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
    // validation
    struct addrinfo host_hints;
    struct addrinfo *host_res;
    memset(&host_hints, 0, sizeof(host_hints));
    host_hints.ai_family = AF_UNSPEC;
    host_hints.ai_socktype = SOCK_STREAM;
    host_hints.ai_flags = AI_PASSIVE;
    int status = getaddrinfo(addr.c_str(), std::to_string(port).c_str(), &host_hints, &host_res);
    if (status < 0)
    {
        std::cerr << "Error: cannot get address info for host"
                  << " getaddrinfo: %s"
                  << gai_strerror(status) << std::endl;
        return -1;
    }
    server_fd = socket(host_res->ai_family, host_res->ai_socktype, host_res->ai_protocol);
    if (server_fd < 0)
    {
        std::cerr << "Error: cannot create socket to server to " << addr << std::endl;
        return -1;
    }

    status = connect(server_fd, host_res->ai_addr, host_res->ai_addrlen);
    if (status == -1)
    {
        std::cerr << "Error: cannot connect to server to " << addr << std::endl;
        return -1;
    }
    std::cout << "Connect to server successfully\n";
    freeaddrinfo(host_res);

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
    // print request
    std::cout << "Request: " << request->getData() << std::endl;

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
        return -1;
    }

    char server_msg[65536] = {0};
    int mes_len = recv(server_fd, server_msg, sizeof(server_msg), 0); // receive response from server

    if (mes_len < 0)
    {
        std::cerr << "Error receiving response from server" << std::endl;
        return -1;
    }

    Response *httpServerResponse = new Response(server_msg);
    // print
    std::cout << "Response: " << httpServerResponse->getData() << std::endl;
    // print isChunked
    std::cout << "isChunked: " << httpServerResponse->isChunked() << std::endl;
    if (httpServerResponse->isChunked())
    {
        send(client_fd_connection, httpServerResponse->getData().c_str(), httpServerResponse->getData().size(), 0); // send response to client

        while (true)
        {
            char chunk[65536] = {0};
            int chunk_len = recv(server_fd, chunk, sizeof(chunk), 0); // receive chunk from server

            // print request
            std::cout << "Chunk: " << chunk << std::endl;
            // print len
            std::cout << "Chunk len: " << chunk_len << std::endl;
            if (chunk_len <= 0)
            {
                // print end
                std::cout << "End of chunk" << std::endl;
                break;
            }
            send(client_fd_connection, chunk, chunk_len, 0); // send chunk to client
            // print successful
            std::cout << "Successful" << std::endl;
        }
    }
    else
    {
        // TODO: Cache

        send(client_fd_connection, httpServerResponse->getData().c_str(), httpServerResponse->getData().size(), 0); // send response to client
    }
    // print isChunked
    // delete buf;
    delete httpServerResponse;
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
