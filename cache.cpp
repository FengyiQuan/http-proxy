#include "cache.hpp"

Cache::Cache()
{
    this->capacity = 50;
    this->cache_map = {};
}
Cache::Cache(unsigned capacity)
{
    this->capacity = capacity;
    this->cache_map = {};
}

Cache::~Cache()
{
}

void Cache::put(std::string url, Response response)
{
    if (cache_map.find(url) == cache_map.end())
    {
        // if (cache_map.size() >= capacity)
        // {
        //     // log
        //     std::ostringstream cacheLog;
        //     cacheLog << "cache is full" << std::endl;
        //     LOG(cacheLog.str());
        //     return;
        // }
        if (checkCacheControl(response))
        {
            cache_map[url] = response;
        }
    }
    // else
    // {
    //     // log
    //     std::ostringstream cacheLog;
    //     cacheLog << "already in cache" << std::endl;
    //     LOG(cacheLog.str());
    // }
}

std::vector<char> Cache::getResponse(std::string url, int requestId)
{
    std::vector<char> response;
    if (cache_map.find(url) != cache_map.end())
    {
        response = cache_map[url]->getData();
    }
    else
    {
        // log
        std::ostringstream cacheLog;
        cacheLog << requestId << ": not in cache" << std::endl;
        LOG(cacheLog.str());
    }
    return response;
}

void Cache::deleteCache(std::string url)
{
    if (cache_map.find(url) != cache_map.end())
    {
        cache_map.erase(url);
    }
}

// check if the response has cache-control
bool Cache::checkCacheControl(Response &response)
{
    std::vector<std::string> cacheControl = response->getCacheControl();
    // || cache_control == "no-cache" || cache_control == "must-revalidate" || cache_control == "max-age=0"
    if (std::find(cacheControl.begin(), cacheControl.end(), "no-store") != cacheControl.end())
    {
        return false;
    }
    return true;
}
