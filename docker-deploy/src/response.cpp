#include "response.hpp"
#include "parser.hpp"

Response::Response() : status_line(), status_code(), data(), headers()
{
}

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

std::string Response::getETag()
{
    if (headers.find("Etag") != headers.end())
    {
        return headers["Etag"];
    }
    return "";
}

std::string Response::getLastModified()
{
    if (headers.find("Last-Modified") != headers.end())
    {
        return headers["Last-Modified"];
    }
    return "";
}

std::string Response::getDate()
{
    if (headers.find("Date") != headers.end())
    {
        return headers["Date"];
    }
    return "";
}

// operator=
Response &Response::operator=(const Response &rhs)
{
    if (this != &rhs)
    {
        this->status_line = rhs.status_line;
        this->status_code = rhs.status_code;
        this->headers = rhs.headers;
        this->data = rhs.data;
    }
    return *this;
}

// add max-age and data together
std::string Response::getExpiredTime()
{

    std::string cache_control = this->getCacheControl();
    std::string max_age = cache_control.substr(cache_control.find("max-age") + 8);
    int max_age_int = atoi(max_age.c_str());
    std::string time_str = this->getDate();

    // get date from header
    if (time_str.find(" GMT") != std::string::npos)
    {
        time_str = time_str.substr(0, time_str.length() - 4);
    }
    // time_t date = 0;
    struct tm tm;
    strptime(time_str.c_str(), "%a, %d %b %Y %H:%M:%S", &tm);
    time_t expire_time = mktime(&tm) + max_age_int;
    struct tm *expire_struct = localtime(&expire_time);
    char buf[80];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", expire_struct);
    std::string expire_str(buf);
    return expire_str;
}
// check max age
bool Response::isExpired()
{
    if (headers.find("Cache-Control") != headers.end())
    {
        std::string cache_control = headers["Cache-Control"];
        if (cache_control.find("max-age") != std::string::npos)
        {
            std::string max_age = cache_control.substr(cache_control.find("max-age") + 8);
            int max_age_int = atoi(max_age.c_str());
            std::string time_str = this->getDate();

            // get date from header
            if (time_str.find(" GMT") != std::string::npos)
            {
                time_str = time_str.substr(0, time_str.length() - 4);
                return false;
            }
            time_t now;
            time(&now);
            struct tm *now_struct = localtime(&now);
            struct tm time_struct;
            strptime(time_str.c_str(), "%a, %d %b %Y %H:%M:%S", &time_struct);
            double seconds = difftime(mktime(&time_struct), mktime(&(*now_struct))) + max_age_int; // end - begin
            return (seconds <= 0);
        }
    }
    return false;
}

// Path: request.cpp
