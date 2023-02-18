//
//  main.cpp
//  proxy
//
//  Created by 全峰毅 on 2/15/23.
//

#include "proxy.hpp"

#include<iostream>
#include<string>
#include<stdlib.h>

#define PORT 12345

int main(int argc,char* argv[]){
    Proxy* proxy;
    port  = atoi(PORT)
    proxy = new Proxy(port);
    
    if(proxy->run()){
            cerr<<"Error starting HTTPProxy"<<endl;
        }
    return 0;
}
