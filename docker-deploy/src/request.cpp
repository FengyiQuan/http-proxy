#include "request.hpp"
#include "parser.hpp"

Request::Request(std::vector<char> request)
{
    data = request;

    Parser *p = new Parser();
    p->parse(request.data(), this);
    delete p;
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

std::string Request::getHost()
{
    return host;
}
size_t Request::getPort()
{
    return port;
}

std::vector<char> Request::getData()
{
    return data;
}

std::string Request::getStartLine()
{
    return start_line;
}

// getURI
std::string Request::getURI()
{
    std::string request_target = this->getStartLine();
    std::string uri = "";
    if (request_target.find("http://") != std::string::npos || request_target.find("https://") != std::string::npos)
    {
        uri = request_target;
    }
    else if (request_target.find("/") != std::string::npos && request_target.find("/") == 0)
    {
        uri = getHost() + request_target;
    }
    else
    {
        uri = getHost() + request_target;
    }
    return uri;
}

void Request::setMethod(std::string method)
{
    this->method = method;
}

void Request::setRequestTarget(std::string request_target)
{
    this->request_target = request_target;
}

void Request::setHeaders(std::map<std::string, std::string> headers)
{
    this->headers = headers;
}

void Request::setHost(std::string host)
{
    this->host = host;
}

void Request::setPort(size_t port)
{
    this->port = port;
}

void Request::setStartLine(std::string start_line)
{
    this->start_line = start_line;
}
