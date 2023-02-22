#include "response.hpp"
#include "parser.hpp"

Response::Response(std::string response)
{
    data = response;

    Parser *p = new Parser();
    p->parse(response, this);
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

std::string Response::getData()
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

// Path: request.cpp
