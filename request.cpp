#include "request.hpp"
#include "parser.hpp"

Request::Request(std::string request)
{
    data = request;

    Parser *p = new Parser();
    p->parse(request, this);
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

std::string Request::getData()
{
    return data;
}


std::string Request::getStartLine()
{
    return start_line;
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


