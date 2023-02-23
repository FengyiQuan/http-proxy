//
//  proxy.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"

Proxy::Proxy(size_t port) : portNum(port)
{
}

// TODO: close socket
Proxy::~Proxy()
{
    close(client_fd);
    // close(client_fd_connection);
    close(server_fd);
}

int Proxy::run()
{
    // client_fd = initSocketServer();
    // init_server(std::to_string(portNum));
    if (initSocketServer() < 0)
    {
        // LOG
        return -1;
    }

    while (true)
    {
        // accept client connection
        socklen_t cliLen = sizeof(clientAddr);
        int client_fd_connection = accept(client_fd, (struct sockaddr *)&clientAddr, &cliLen);

        if (client_fd_connection < 0)
        {
            // LOG
            std::cerr << "Error accepting client connection" << std::endl;
            return -1;
        }

        std::thread(&Proxy::handleRequest, client_fd_connection).detach();
        // t.detach();

        // spawn thread to handle request
        // if (handleRequest())
        // {
        //     return -1;
        // }

        // close(client_fd_connection);
        // return 0;
        return 0;
    }

    return 0;
}

// init connection between client and proxy
int Proxy::initSocketServer()
{
    // client_fd = init_server(std::to_string(portNum));
    // return 0;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        // LOG
        std::cerr << "Error creating client socket" << std::endl;
        return -1;
    }

    // bind to port 12345
    int opt = 1;
    setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(portNum);
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
int Proxy::initSocketClient(std::string address, size_t port)
{
    // server_fd = init_client(address, std::to_string(port));
    // return 0;
    // validation
    struct addrinfo host_hints;
    struct addrinfo *host_res;
    memset(&host_hints, 0, sizeof(host_hints));
    host_hints.ai_family = AF_UNSPEC;
    host_hints.ai_socktype = SOCK_STREAM;
    // host_hints.ai_flags = AI_PASSIVE;
    // int status = getaddrinfo(addr.c_str(), std::to_string(port).c_str(), &host_hints, &host_res);
    int status = getaddrinfo(address.c_str(), my_to_string(port).c_str(), &host_hints, &host_res);
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
        std::cerr << "Error: cannot create socket to server to " << address << std::endl;
        return -1;
    }

    status = connect(server_fd, host_res->ai_addr, host_res->ai_addrlen);
    if (status == -1)
    {
        std::cerr << "Error: cannot connect to server to " << address << std::endl;
        return -1;
    }
    std::cout << "Connect to server successfully\n";
    freeaddrinfo(host_res);

    return 0;
}

int Proxy::handleRequest(int client_fd_connection)
{

    //    httpClientResponse= new HTTPResponse();
    //    httpServerRequest = new HTTPRequest();
    //    httpServerResponse = new HTTPResponse();
    // std::cout << "hello world" << std::endl;

    // int buf_sz = 65536;
    // char *buf = new char[buf_sz];
    // memset(buf, '\0', buf_sz);
    std::vector<char> req_msg(BUF_LEN);
    ssize_t recvLength = recv(client_fd_connection, &req_msg.data()[0], BUF_LEN, 0);
    // char req_msg[65536] = {0};
    // int recvLength = recv(client_fd_connection, req_msg, sizeof(req_msg), 0); // fisrt request from client

    if (recvLength < 0)
    {
        // LOG
        std::cerr << "Error receiving client request" << std::endl;
        return -1;
    }
    // std::cout << "hello world" << std::endl;
    Request *httpClientRequest = new Request(req_msg.data());
    if (httpClientRequest->getMethod() == "CONNECT")
    {
        handleConnect(httpClientRequest, client_fd_connection);
    }
    else if (httpClientRequest->getMethod() == "GET")
    {
        handleGet(httpClientRequest, client_fd_connection);
    }
    else if (httpClientRequest->getMethod() == "POST")
    {
        handlePost(httpClientRequest, client_fd_connection);
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
int Proxy::handleConnect(Request *request, int client_fd_connection)
{
    // make socket connection to server

    // if (initSocketServer(request->getHost(), request->getPort()) < 0)
    // {
    //     // LOG
    //     return -1;
    // }
    // print request
    std::cout << "Request: " << request->getData() << std::endl;

    return 0;
}

// handleGet from client and server
int Proxy::handleGet(Request *request, int client_fd_connection)
{
    std::cout << "this is a get request" << std::endl;
    initSocketClient(request->getHost(), request->getPort());
    // server_fd = init_client(request->getHost(), std::to_string(request->getPort()));
    // print
    // std::cout << "init_client: " << std::endl;
    if (server_fd < 0)
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
    std::vector<char> server_msg(BUF_LEN);
    ssize_t server_mes_len = recv(server_fd, &server_msg.data()[0], BUF_LEN, 0);
    // print
    // std::cout << "server_msg---------" << std::endl
    //           << server_msg.data() << std::endl;
    // char server_msg[65536] = {0};
    // int mes_len = recv(server_fd, server_msg, sizeof(server_msg), 0); // receive response from server
    if (server_mes_len == 0)
    {
        return 0;
    }
    // std::cout << "server_msg---------" << std::endl
    //           << server_msg.data() << std::endl;
    if (server_mes_len < 0)
    {
        std::cerr << "Error receiving response from server" << std::endl;
        return -1;
    }

    Response *httpServerResponse = new Response(server_msg.data());
    // print everything
    // std::cout << "Response Object----:" << std::endl
    //           << httpServerResponse->getData() << std::endl;
    // // print status line
    // std::cout << "Status Line----:" << std::endl
    //           << httpServerResponse->getStatusLine() << std::endl;
    // // print status code
    // std::cout << "Status Code----:" << std::endl
    //           << httpServerResponse->getStatusCode() << std::endl;
    // // print isChunked
    // std::cout << "isChunked: " << httpServerResponse->isChunked() << std::endl;
    if (httpServerResponse->isChunked())
    {
        if (send(client_fd_connection, server_msg.data(), server_mes_len, 0) < 0)
        {

            std::cout << "send httpServerResponse response to client failed" << std::endl;
        }
        else
        {
            std::cout << "send response to client successfully" << std::endl
                      << server_msg.data() << std::endl;
        }
        std::vector<char> chunked_msg(BUF_LEN);
        // ssize_t server_mes_len = recv(client_fd_connection, &server_msg.data()[0], BUF_LEN, 0);
        while (true)
        { // receive and send remaining message
            int len = recv(server_fd, chunked_msg.data(), sizeof(chunked_msg), 0);
            // if (len <= 0)
            // {
            //     // TODO: never go here!!!
            //     std::cout << "chunked break\n";
            //     break;
            // }
            if (send(client_fd_connection, chunked_msg.data(), len, 0) < 0)
            {
                std::cout << "WSAGetLastError: " << strerror(errno) << std::endl;
                std::cout << "send chunked_msg to client failed" << std::endl;
            }
            else
            {
                std::cout << "send chunked_msg to client successfully" << std::endl;
            }
            // if end of chunk, break
            std::string temp(chunked_msg.data());
            if (temp.find("0\r\n\r\n") != std::string::npos || temp.find("\r\n\r\n") != std::string::npos || len <= 0)
            {
                std::cout << "chunked break\n";
                break;
            }
        }
    }
    else
    {
        // TODO: Cache
        // send response to client
        long int send_status = send(client_fd_connection, httpServerResponse->getData().c_str(), httpServerResponse->getData().size(), 0);
        if (send_status == -1)
        {
            std::cout << "send failed" << std::endl;
        }
        else
        {
            std::cout << "send successful: send_status is: " << send_status << std::endl;
        }
    }
    // std::cout << "finish now" << std::endl;
    // print isChunked
    // delete buf;
    delete httpServerResponse;
    return 0;
}

int Proxy::handlePost(Request *request, int client_fd_connection)
{
    std::cout << "this is a post request" << std::endl;
    initSocketClient(request->getHost(), request->getPort());
    if (server_fd < 0)
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
    else
    {
        std::cout << "send request to server successfully" << std::endl;
    }
    std::vector<char> server_msg(BUF_LEN);
    ssize_t server_mes_len = recv(server_fd, &server_msg.data()[0], BUF_LEN, 0);

    if (server_mes_len <= 0)
    {
        std::cerr << "Error receiving response from server" << std::endl;
        return -1;
    }

    if (send(client_fd, server_msg.data(), server_mes_len, 0) < 0)
    {
        std::cout << "send httpServerResponse response to client failed" << std::endl;
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
