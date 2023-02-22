#ifndef response_hpp
#define response_hpp

#include <string>
#include <map>
#include <stdlib.h>

class Response
{
public:
    Response(std::string response);

    // getter
    std::string getStatusLine();
    std::string getStatusCode();
    std::map<std::string, std::string> getHeaders();
    // setter
    void setStatusLine(std::string status_line);
    void setStatusCode(std::string status_code);
    void setHeaders(std::map<std::string, std::string> headers);

private:
    std::string status_line;
    std::string status_code;
    std::string data;

    std::map<std::string, std::string> headers;
    std::string body;

    // std::string version; // http/1.1
    // std::string buf;
    // std::string log_info;
    // std::string send;
    // std::string input;
    // std::string line;
};

#endif /* response_hpp */
