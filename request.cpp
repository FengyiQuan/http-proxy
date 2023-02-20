#include "request.hpp"
#include <iostream>
Request::Request(std::string request)
{
    parseStartLine(request);
    parseHeaders(request);
}

void Request::parseStartLine(std::string request)
{
    size_t pos = request.find("\r\n");
    start_line = request.substr(0, pos);
    size_t firstSP = start_line.find(" ");
    size_t lastSP = start_line.rfind(" ");
    method = start_line.substr(0, firstSP);
    request_target = start_line.substr(firstSP + 1, lastSP - firstSP - 1);
}
void Request::parseHeaders(std::string request)
// header-field   = field-name ":" OWS field-value OWS
{
    // print request
    // std::cout << "requesst: " << request << std::endl;
    size_t pos = request.find("\r\n");
    std::string header = request.substr(pos + 2);
    // std::cout << "start at " << pos << ", start:" << header << std::endl;
    std::string line;
    // std::cout << "parse header start" << std::endl;
    while ((pos = header.find("\r\n")) != std::string::npos)
    {
        // std::cout << "start at " << pos << ", start:" << header << std::endl;
        line = header.substr(0, pos);
        size_t colon = line.find(":");
        std::string key = line.substr(0, colon);

        std::string value = line.substr(colon + 1);
        value.erase(0, value.find_first_not_of("\t\n\v\f\r ")); // left trim    
        value.erase(value.find_last_not_of("\t\n\v\f\r ") + 1); // right trim
        if (!(key == "" && value == ""))
        {
            headers[key] = value;
        } // insert key/value pair to struct
        // std::cout << key << ": " << value << std::endl;
        header = header.substr(pos + 2);
    }
    // std::map<std::string, std::string>::iterator it;
    // for (it = getHeaders().begin(); it != getHeaders().end(); it++)
    // {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
}

std::string Request::getMethod()
{
    return method;
}

std::string Request::getRequestTarget()
{
    return request_target;
}

std::map<std::string, std::string> Request::getHeaders()
{
    return headers;
}
