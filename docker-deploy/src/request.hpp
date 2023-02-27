#ifndef request_hpp
#define request_hpp

#include <string>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <vector>
// #include "parser.hpp"
class Request
{
public:
    Request(std::vector<char> request);

    // getter
    std::string getMethod();
    std::string getRequestTarget();
    std::map<std::string, std::string> getHeaders();
    std::string getHost();
    size_t getPort();
    std::vector<char> getData();
    std::string getStartLine();
    std::string getURI();
    // std::map<std::string, std::string> getBody();
    // setter
    void setStartLine(std::string start_line);
    void setMethod(std::string method);
    void setRequestTarget(std::string request_target);
    void setHeaders(std::map<std::string, std::string> headers);
    void setHost(std::string host);
    void setPort(size_t port);
    // void setBody(std::string body);

private:
    std::string start_line;
    std::string method;
    std::string request_target;
    // std::string protocol;
    std::string host;
    size_t port;
    std::vector<char> data;
    // std::string path;
    std::map<std::string, std::string> headers;

    // TODO: body
    // std::string body;

    // std::string version; // http/1.1
    // std::string buf;
    // std::string log_info;
    // std::string send;
    // std::string input;
    // std::string line;
};

#endif /* request_hpp */
