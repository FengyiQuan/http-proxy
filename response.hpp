#ifndef response_hpp
#define response_hpp

#include <string>
#include <map>
#include <stdlib.h>
#include <vector>

class Response
{
public:
    Response(std::vector<char> response);

    // getter
    std::string getStatusLine();
    std::string getStatusCode();
    std::map<std::string, std::string> getHeaders();
    std::vector<char> getData();

    std::string getCacheControl();
    // setter
    void setStatusLine(std::string status_line);
    void setStatusCode(std::string status_code);
    void setHeaders(std::map<std::string, std::string> headers);
    bool isChunked();
    // setCacheControl

    // void setCacheControl(std::vector<std::string> cache_control);

    

private:
    std::string status_line;
    std::string status_code;
    std::vector<char> data;

    std::map<std::string, std::string> headers;
    // cache control
    // std::vector<std::string> cache_control;
    // std::string body;

    // std::string version; // http/1.1
    // std::string buf;
    // std::string log_info;
    // std::string send;
    // std::string input;
    // std::string line;
};

#endif /* response_hpp */
