//
//  main.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>

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
