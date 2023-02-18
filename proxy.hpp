//
//  proxy.hpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#ifndef proxy_hpp
#define proxy_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    // int initSocketServer(void);

    int handleRequest(void);

    // int recvRequestClient(void);
    // int sendRequestServer(void);
    int client_fd;
    struct sockaddr_in proxyAddr, clientAddr;
};

#endif /* proxy_hpp */
