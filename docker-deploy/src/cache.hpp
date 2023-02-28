#ifndef cache_cpp
#define cache_cpp

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "response.hpp"
#include "request.hpp"
#include "logger.hpp"

extern std::mutex cacheMux;
class Cache
{
private:
    unsigned int capacity;
    // std::vector<std::string> url_vector{};
    std::map<std::string, Response> cache_map; // url, response
    bool checkCacheControl(Response &response, int requestId);
    void deleteCache(std::string url);

public:
    Cache();
    Cache(unsigned capacity);
    ~Cache();
    void put(std::string url, Response response, int requestId);
    std::vector<char> getResponse(std::string url, int requestId);
    bool isInCache(std::string url);
    bool needRevalidate(std::string url, int requestId);
    std::string generateValidateRequest(std::string url, Request &request);
    // int getCapacity();
    // LRUCache() : capacity(0) {}
};

#endif
