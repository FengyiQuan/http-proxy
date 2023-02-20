#ifndef request_hpp
#define request_hpp

#include <string>
#include <map>

class Request
{
public:
    Request(std::string request);

    // getter
    std::string getMethod();
    std::string getRequestTarget();
    std::map<std::string, std::string> getHeaders();


private:
    std::string start_line;
    std::string method;
    std::string request_target;
    // std::string protocol;
    // std::string host;
    // std::string port;
    // std::string path;
    std::map<std::string, std::string> headers;

    void parseStartLine(std::string request);
    void parseHeaders(std::string request);



    // std::string version; // http/1.1
    // std::string buf;
    // std::string log_info;
    // std::string send;
    // std::string input;
    // std::string line;
};

#endif /* request_hpp */

