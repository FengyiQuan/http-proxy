#include "response.hpp"
#include "parser.hpp"

Response::Response(std::vector<char> response)
{
    this->data = response;
    // cache_control = std::vector<std::string>();
    Parser *p = new Parser();
    p->parse(response.data(), this);
    delete p;
}

// isChunked
bool Response::isChunked()
{
    if (headers.find("Transfer-Encoding") != headers.end())
    {
        if (headers["Transfer-Encoding"] == "chunked")
        {
            return true;
        }
    }
    return false;
}

std::vector<char> Response::getData()
{
    return data;
}
std::string Response::getStatusLine()
{
    return status_line;
}

std::string Response::getStatusCode()
{
    return status_code;
}

std::map<std::string, std::string> Response::getHeaders()
{
    return headers;
}

std::string Response::getCacheControl()
{
    if (headers.find("Cache-Control") != headers.end())
    {
        return headers["Cache-Control"];
    }
    return "";
}

void Response::setStatusLine(std::string status_line)
{
    this->status_line = status_line;
}

void Response::setStatusCode(std::string status_code)
{
    this->status_code = status_code;
}

void Response::setHeaders(std::map<std::string, std::string> headers)
{
    this->headers = headers;
}

// void Response::setCacheControl(std::vector<std::string> cache_control)
// {
//     this->cache_control = cache_control;
// }

// Path: request.cpp
