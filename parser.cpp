#include "parser.hpp"


void Parser::parse(std::string data, Request *request)
{
    bool type = REQUEST;
    parseStartLine(data, request, type);
    parseHeaders(data, request, type);
    parseBody(data, request, type);
}

void Parser::parse(std::string data, Response *response)
{
    bool type = RESPONSE;
    parseStartLine(data, response, type);
    parseHeaders(data, response, type);
    parseBody(data, response, type);
}

void Parser::parseStartLine(std::string data, void *request, bool type)
{
    if (type == REQUEST)
    {
        // request-line = method SP request-target SP HTTP-version CRLF
        Request *r = (Request *)request;
        size_t pos = data.find("\r\n");
        r->setStartLine(data.substr(0, pos));
        std::string start_line = r->getStartLine();
        size_t firstSP = start_line.find(" ");
        size_t lastSP = start_line.rfind(" ");
        r->setMethod(start_line.substr(0, firstSP));
        r->setRequestTarget(start_line.substr(firstSP + 1, lastSP - firstSP - 1));
    }
    else
    {
        // status-line = HTTP-version SP status-code SP reason-phrase CRLF
        Response *r = (Response *)request;
        size_t pos = data.find("\r\n");
        r->setStatusLine(data.substr(0, pos));
        std::string status_line = r->getStatusLine();
        size_t firstSP = status_line.find(" ");
        size_t secondSP = status_line.find(" ", firstSP + 1);
        r->setStatusCode(status_line.substr(firstSP + 1, secondSP - firstSP - 1));
    }
}

// header-field   = field-name ":" OWS field-value OWS
void Parser::parseHeaders(std::string data, void *request, bool type)
{

    // print request
    // std::cout << "requesst: " << request << std::endl;
    size_t pos = data.find("\r\n");
    std::string header = data.substr(pos + 2);
    // std::cout << "start at " << pos << ", start:" << header << std::endl;
    std::string line;
    // std::cout << "parse header start" << std::endl;
    std::map<std::string, std::string> headers;
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
            if (key == "Host")
            {
                Request *r = (Request *)request;
                size_t colon = value.find(":");
                if (colon != std::string::npos)
                {
                    r->setHost(value.substr(0, colon));
                    r->setPort(atoi(value.substr(colon + 1).c_str()));
                }
                else
                {
                    r->setHost(value);
                    r->setPort(80);
                }
            }
            headers[key] = value;
        } // insert key/value pair to struct
        // std::cout << key << ": " << value << std::endl;
        header = header.substr(pos + 2);
    }
    if (type == REQUEST)
    {
        Request *r = (Request *)request;
        r->setHeaders(headers);
    }
    else
    {
        Response *r = (Response *)request;
        r->setHeaders(headers);
    }
    // std::map<std::string, std::string>::iterator it;
    // for (it = getHeaders().begin(); it != getHeaders().end(); it++)
    // {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
}

void Parser::parseBody(std::string data, void *request, bool type)
{
    // message-body = *OCTET
    // size_t pos = data.find("\r\n\r\n");
    // std::string body = data.substr(pos + 4);
    // std::map<std::string, std::string> body;
    // parse into key value pari and store into body
        

}

