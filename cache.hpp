#ifndef cache_cpp
#define cache_cpp

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "response.hpp"
#include "logger.hpp"

class Cache
{
private:
    unsigned int capacity;
    // std::vector<std::string> url_vector{};
    std::map<std::string, Response> cache_map; //
    bool checkCacheControl(Response &response);
    void deleteCache(std::string url);

public:
    Cache();
    Cache(unsigned capacity);
    ~Cache();
    void put(std::string url, Response response);
    std::vector<char> getResponse(std::string url, int requestId);
    // int getCapacity();
    // LRUCache() : capacity(0) {}
};

#endif
