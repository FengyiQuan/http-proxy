#ifndef parser_hpp
#define parser_hpp

#include <string>
#include <map>
#include <stdlib.h>
#include "request.hpp"
#include "response.hpp"

#define REQUEST false
#define RESPONSE true

class Parser
{
public:
    // request is request or response that hold the value
    void parse(std::string data, Request *request);
    void parse(std::string data, Response *response);

private:
    void parseStartLine(std::string data, void *request, bool type);
    void parseHeaders(std::string data, void *request, bool type);
    void parseBody(std::string data, void *request, bool type);
};

#endif /* parser_hpp */
