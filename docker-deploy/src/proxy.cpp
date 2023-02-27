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
    Cache *cache = new Cache();

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
        struct sockaddr_storage socket_addr;
        // socklen_t socket_addr_len = sizeof(socket_addr);
        struct sockaddr_in *addr = (struct sockaddr_in *)&socket_addr;
        std::string ip = inet_ntoa(addr->sin_addr);
        // print
        // std::cout << "client_fd_connection: " << client_fd_connection << std::endl;
        // create thread object
        int requestId = 0;
        // ThreadObject threadObject(ip, requestId++, client_fd_connection);

        // std::   (&Proxy::handleRequest, this, &threadObject).detach();
        // print new thread
        std::cout << "new thread: " << requestId << std::endl;
        std::thread(&Proxy::handleRequest, this, client_fd_connection, ip, requestId, cache).detach();
        requestId += 1;

        // // sleep(5000);
        // // print
        // std::cout << "after client_fd_connection: " << client_fd_connection << std::endl;

        // t.detach();

        // spawn thread to handle request
        // if (handleRequest(&threadObject))
        // {
        //     return -1;
        // }

        // close(client_fd_connection);
        // return 0;
        // return 0;
    }

    return 0;
}

// init connection between client and proxy, called on proxy
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

// socket conect to server, clled on spawned thread
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
    int server_fd = socket(host_res->ai_family, host_res->ai_socktype, host_res->ai_protocol);
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

    return server_fd;
}

// entry point for handle request
int Proxy::handleRequest(int client_fd_connection, std::string ip, size_t requestId, Cache *cache)
{
    // int client_fd_connection = threadObject->getClientConnectionFd();
    // std::string ip = threadObject->getIp();
    // size_t requestId = threadObject->getRequestId();
    // print ip
    // std::cout << "ip: " << ip << std::endl;
    try
    {
        while (true)
        {
            std::vector<char> req_msg(BUF_LEN);
            recv_data(client_fd_connection, req_msg);
            Request *httpClientRequest = new Request(req_msg);
            // log
            std::ostringstream requestLog;
            requestLog << requestId << ": \"" << httpClientRequest->getStartLine() << "\" from " << ip << " @ " << now();
            LOG(requestLog.str());
            if (httpClientRequest->getMethod() == "CONNECT")
            {
                handleConnect(httpClientRequest, client_fd_connection, requestId);

                // log
                // std::ostringstream closeLog;
                // std::cout << "Tunnel closed" << std::endl;
                // closeLog << requestId << ": Tunnel closed";
            }
            else if (httpClientRequest->getMethod() == "GET")
            {
                handleGet(httpClientRequest, client_fd_connection, requestId, cache);
            }
            else if (httpClientRequest->getMethod() == "POST")
            {
                handlePost(httpClientRequest, client_fd_connection, requestId);
            }
            else
            {
                // LOG
                // std::cerr << req_msg.data() << std::endl;
                // std::cerr << "Error: unknown method: " << httpClientRequest->getStartLine() << std::endl;
                // log 404
                // send 404 to client
                std::string temp("HTTP/1.1 404 Not Found\r\n\r\n");
                std::vector<char> v(temp.begin(), temp.end());
                send_data(client_fd_connection, v, v.size());
                std::ostringstream requestLog;
                requestLog << requestId << ": Responding \""
                           << "HTTP/1.1 404 Not Found\""
                           << std::endl;
                LOG(requestLog.str());
                delete httpClientRequest;
                // std::vector<char> temp("HTTP/1.1 404 Not Found");
                // send_data(client_fd_connection, "HTTP/1.1 404 Not Found", "HTTP/1.1 404 Not Found\"".size());
                return -1;
            }
            delete httpClientRequest;
        }
    }
    catch (std::invalid_argument const &ex)
    {
        if (ex.what() == std::to_string(client_fd))
        {
            // send 400 to client
            send_data(client_fd_connection, BAD_REQUEST, BAD_REQUEST.size());
        }
        else
        {
            // send 502 to client
            send_data(client_fd_connection, BAD_GATEWAY, BAD_GATEWAY.size());
        }
    }
    catch (std::exception const &e)
    {

        // send 400 to client
        // send bad request
        send_data(client_fd_connection, BAD_REQUEST, BAD_REQUEST.size());

        handleRequest(client_fd_connection, ip, requestId, cache);
        // std::cerr << "test: " << e.what() << '\n';
        // LOG(e.what());
    }
    // close(client_fd_connection);
    return 0;
}

// handleConnect
int Proxy::handleConnect(Request *request, int client_fd_connection, int requestId)
{
    // make socket connection to server
    // send 200 OK to client

    // send_data(client_fd_connection, OK, OK.size());

    int server_fd = initSocketClient(request->getHost(), request->getPort());
    // if (server_fd < 0)
    // {
    //     // send 400 to client
    //     std::cerr << "Error: cannot connect to server" << std::endl;
    //     return -1;
    // }
    send(client_fd_connection, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);
    std::ostringstream requestLog;
    requestLog << requestId << ": Responding \""
               << "HTTP/1.1 200 OK"
               << "\"";
    LOG(requestLog.str());

    // print start tunnel
    std::cout << "Start tunneling " << std::endl;

    fd_set fds;
    int nfds = std::max(client_fd_connection, server_fd);
    while (true)
    {
        // std::cout << "loop" << std::endl;
        FD_ZERO(&fds);
        FD_SET(client_fd_connection, &fds);
        FD_SET(server_fd, &fds);
        int ret = select(nfds + 1, &fds, NULL, NULL, NULL);
        if (ret < 0)
        {
            std::cerr << "Error: select error" << std::endl;
            return -1;
        }
        if (FD_ISSET(server_fd, &fds))
        {
            std::vector<char> server_data(BUF_LEN);

            // std::cout << "waiting for server msg: " << std::endl;
            // int server_data_len = recv_data(server_fd, server_data);
            // print data

            int server_data_len = recv(server_fd, &server_data.data()[0], BUF_LEN, 0);
            if (server_data_len <= 0)
            {
                return 0;
            }
            // std::cout << "recived from server,len: " << server_data_len << std::endl;
            // Response *serverResponse = new Response(server_data.data());
            // // log received
            // std::ostringstream receivedLog;
            // receivedLog << requestId << ": Received \"" << serverResponse->getStatusLine() << "\" from " ;
            // LOG(requestLog.str());
            // if (server_data_len == 0)
            // {
            //     close(client_fd_connection);
            //     close(server_fd);
            //     return 0;
            // }
            if (send(client_fd_connection, server_data.data(), server_data_len, 0) <= 0)
            {
                return -1;
            }
            // send_data(client_fd_connection, server_data, server_data_len);
        }
        if (FD_ISSET(client_fd_connection, &fds))
        {
            std::vector<char> client_data(BUF_LEN);
            // std::cout << "waiting for client msg: " << std::endl;
            // int client_data_len = recv_data(client_fd_connection, client_data);
            int client_data_len = recv(client_fd_connection, &client_data.data()[0], BUF_LEN, 0);
            // std::cout << "recived for client msg, len: " << client_data_len << std::endl;
            // Request *clientRequest = new Request(client_data.data());
            // log
            // std::ostringstream requestLog;
            // requestLog << requestId << ": \"" << clientRequest->getStartLine() << "\" from " << clientRequest->getHost() << " @ " << now();
            // LOG(requestLog.str());
            if (client_data_len == 0)
            {
                // close(server_fd);
                // close(client_fd_connection);
                return 0;
            }
            // send_data(server_fd, client_data, client_data_len);
            if (send(server_fd, client_data.data(), client_data_len, 0) <= 0)
            {
                return -1;
            }
            // log requesting
            // std::ostringstream requestLog;
            // requestLog << requestId << ": Requesting \"" << clientRequest->getStartLine() << "\" from" << clientRequest->getHost();
            // LOG(requestLog.str());
        }
    }
    // log ID: Tunnel closed
    std::ostringstream closeLog;
    // print close
    std::cout << "Tunnel closed" << std::endl;
    closeLog << requestId << ": Tunnel closed";
    LOG(closeLog.str());
    return 0;
}

// handleGet from client and server
int Proxy::handleGet(Request *request, int client_fd_connection, int requestId, Cache *cache)
{
    // std::cout << "this is a get request" << std::endl;
    int server_fd = initSocketClient(request->getHost(), request->getPort());
    try
    {

        if (server_fd < 0)
        {
            // LOG
            std::cerr << "Error: cannot connect to server" << std::endl;
            return -1;
        }

        std::vector<char> requestData = request->getData();
        std::string requestLine = request->getStartLine();
        std::string requestHost = request->getHost();
        // if cache hit
        bool isInCache = cache->isInCache(requestLine);
        if (isInCache)
        {
            // print
            bool needRevalidate = cache->needRevalidate(requestLine, requestId);
            // print needRevalidate
            std::cout << "needRevalidate: " << needRevalidate << std::endl;

            if (needRevalidate)
            {
                // send validate request
                std::string validateRequest = cache->generateValidateRequest(requestLine, *request);
                std::vector<char> temp_data(validateRequest.begin(), validateRequest.end());
                send_data(server_fd, temp_data, temp_data.size());
                std::vector<char> server_msg(BUF_LEN);
                recv_data(server_fd, server_msg);
                Response *httpServerResponse = new Response(server_msg);
                // if 304, return cache, else return new response
                if (httpServerResponse->getStatusCode() == "304")
                {
                    // send cache to client
                    std::vector<char> cacheData = cache->getResponse(requestLine, requestId);
                    send_data(client_fd_connection, cacheData, cacheData.size());
                }
                else if (httpServerResponse->getStatusCode() == "200")
                {
                    // send new response to client
                    send_data(client_fd_connection, server_msg, server_msg.size());
                    // update cache
                    cache->put(requestLine, server_msg, requestId);
                }
            }
            else
            { // log ID: in cache, valid
                std::ostringstream inCacheLog;
                inCacheLog << requestId << ": \"" << requestLine << "\" in cache, valid";
                LOG(inCacheLog.str());
                // send cache to client
                std::vector<char> cacheData = cache->getResponse(requestLine, requestId);
                // std::cout << "cache data: " << cacheData.data() << std::endl;
                send_data(client_fd_connection, cacheData, cacheData.size());
                // log Responding
                std::ostringstream respondingLog;
                respondingLog << requestId << ": Responding \"" << requestLine << "\" from cache";
                LOG(respondingLog.str());
            }
        }

        else
        {
            // log not in cache
            std::ostringstream notInCacheLog;
            notInCacheLog << requestId << ": \"" << requestLine << "\" not in cache";
            LOG(notInCacheLog.str());

            std::ostringstream requestingLog;
            requestingLog << requestId << ": Requesting \"" << requestLine << "\" from " << requestHost;
            LOG(requestingLog.str());

            std::vector<char> temp_data(requestData.begin(), requestData.end());
            // std::copy(requestData.begin(), requestData.end(), temp_data.begin());
            send_data(server_fd, temp_data, temp_data.size());

            std::vector<char> server_msg(BUF_LEN);
            int recvLen = recv_data(server_fd, server_msg);
            Response *httpServerResponse = new Response(server_msg);

            std::ostringstream receivedLog;
            receivedLog << requestId << ": Received \"" << httpServerResponse->getStatusLine() << "\" from " << requestHost;
            LOG(receivedLog.str());

            // log  NOTE Cache-Control:
            std::string cacheControl = httpServerResponse->getCacheControl();
            if (cacheControl != "")
            {
                std::ostringstream cacheControlLog;
                cacheControlLog << requestId << ": NOTE Cache-Control: " << cacheControl;
                LOG(cacheControlLog.str());
            }
            // log NOTE ETag:
            std::string eTag = httpServerResponse->getETag();
            if (eTag != "")
            {
                std::ostringstream eTagLog;
                eTagLog << requestId << ": NOTE ETag: " << eTag;
                LOG(eTagLog.str());
            }

            if (httpServerResponse->isChunked())
            {
                send_data(client_fd_connection, server_msg, recvLen);

                std::ostringstream respondingLog;
                respondingLog << requestId << ": Responding \"" << httpServerResponse->getStatusLine() << "\"";
                LOG(respondingLog.str());

                while (true)
                { // receive and send remaining message
                    std::vector<char> chunked_msg(BUF_LEN);
                    // int len = recv(server_fd, chunked_msg.data(), BUF_LEN, 0);
                    int len = recv_data(server_fd, chunked_msg);
                    send_data(client_fd_connection, chunked_msg, len);
                    std::string temp(chunked_msg.data());
                    // if end of chunk, break
                    if (temp.length() <= 0 || temp.find("0\r\n\r\n") != std::string::npos || temp.find("\r\n\r\n") != std::string::npos)
                    {
                        std::cout << "chunked break\n";
                        break;
                    }
                }
            }
            else
            { // not chunked
                send_data(client_fd_connection, server_msg, recvLen);
                std::ostringstream respondingLog;
                respondingLog << requestId << ": Responding \"" << httpServerResponse->getStatusLine() << "\"";
                LOG(respondingLog.str());
                cache->put(requestLine, server_msg, requestId);
            }
            delete httpServerResponse;
        }
    }

    catch (const std::exception &e)
    {
        // delete httpServerResponse;
        close(server_fd);
        // std::cerr << e.what() << '\n';
    }

    close(server_fd);
    return 0;
}

int Proxy::handlePost(Request *request, int client_fd_connection, int requestId)
{
    std::cout << "this is a post request" << std::endl;
    int server_fd = initSocketClient(request->getHost(), request->getPort());

    if (server_fd < 0)
    {
        // LOG
        std::cerr << "Error: cannot connect to server" << std::endl;
        return -1;
    }

    std::vector<char> requestData = request->getData();
    std::string requestLine = request->getStartLine();
    std::string requestHost = request->getHost();
    // std::vector<char> temp_data(requestData.begin(), requestData.end());
    // std::copy(requestData.begin(), requestData.end(), temp_data.begin());
    send_data(server_fd, requestData, requestData.size());

    std::vector<char> server_msg(65536);
    int recvLen = recv_data(server_fd, server_msg);

    Response *httpServerResponse = new Response(server_msg);

    std::ostringstream receivedLog;
    receivedLog << requestId << ": Received \"" << httpServerResponse->getStatusLine() << "\" from " << requestHost;
    LOG(receivedLog.str());

    send_data(client_fd_connection, server_msg, recvLen);
    std::ostringstream respondingLog;
    respondingLog << requestId << ": Responding \"" << httpServerResponse->getStatusLine() << "\"";
    LOG(respondingLog.str());
    delete httpServerResponse;

    return 0;
}

// bool Proxy::revalidate(Request *request, int server_fd, int requestId, Cache *cache)
// {
//     // get header
//     std::map<std::string, std::string> headers = request->getHeaders();
//     if (headers.find("Etag") == headers.end() || headers.find("Last-Modified") == headers.end())

//     {
//         return true;
//     }
//     return false;
// }

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
