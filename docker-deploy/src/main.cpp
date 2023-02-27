//
//  main.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "main.hpp"
#include "proxy.hpp"



#define PORT "12345" // proxy port

int main(int argc, char *argv[])
{
    Proxy *proxy;
    int port = atoi(PORT);
    proxy = new Proxy(port);

    if (proxy->run())
    {
        std::cerr << "Error starting HTTPProxy" << std::endl;
    }

    delete proxy;
    return 0;
}
