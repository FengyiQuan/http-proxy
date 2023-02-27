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

void Cache::put(std::string url, Response response, int requestId)
{

    // if (cache_map.size() >= capacity)
    // {
    //     // log
    //     std::ostringstream cacheLog;
    //     cacheLog << "cache is full" << std::endl;
    //     LOG(cacheLog.str());
    //     return;
    // }
    if (checkCacheControl(response, requestId))
    { // key = host + path
        // std::string key = response.getHost() + response.getPath();
        // log store cache
        std::ostringstream cacheLog;
        cacheLog << requestId << ": store cache for " << url << std::endl;
        LOG(cacheLog.str());
        cache_map[url] = response;
    }
}

std::vector<char> Cache::getResponse(std::string url, int requestId)
{
    std::vector<char> response;
    if (cache_map.find(url) != cache_map.end())
    {
        // Response response = cache_map[url];
        response = cache_map[url].getData();
        // print
        // std::cout << "response: " << response.data() << std::endl;
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
// false if no-store
// else true: need to store in the cache
bool Cache::checkCacheControl(Response &response, int requestId)
{
    std::string cacheControl = response.getCacheControl();
    // || cache_control == "no-cache" || cache_control == "must-revalidate" || cache_control == "max-age=0"
    if (cacheControl.find("no-store") != std::string::npos)
    {
        // log ID: not cacheable because REASON
        std::ostringstream cacheLog;
        cacheLog << requestId << ": not cacheable because no-store" << std::endl;
        LOG(cacheLog.str());

        return false;
    }
    if (cacheControl.find("no-cache") != std::string::npos || cacheControl.find("must-revalidate") != std::string::npos)
    {
        // log ID: not cacheable because REASON
        std::ostringstream cacheLog;
        cacheLog << requestId << ": cached, but requires re-validation" << std::endl;
        LOG(cacheLog.str());
    }
    if (cacheControl.find("max-age") != std::string::npos)
    {
        std::ostringstream cacheLog;
        cacheLog << requestId << ": cached, expires at " << response.getExpiredTime() << std::endl;
        LOG(cacheLog.str());
    }

    return true;
}

bool Cache::isInCache(std::string url)
{
    if (cache_map.find(url) != cache_map.end())
    {
        return true;
    }
    return false;
}

bool Cache::needRevalidate(std::string url, int requestId)
{
    if (cache_map.find(url) != cache_map.end())
    {
        Response response = cache_map[url];
        std::string cacheControl = response.getCacheControl();
        // must-revalidate or max-age=0 or expired
        if (cacheControl.find("must-revalidate") != std::string::npos || cacheControl.find("max-age=0") != std::string::npos)
        {
            // log ID: in cache, requires validation
            std::ostringstream cacheLog;
            cacheLog << requestId << ": in cache, requires validation" << std::endl;
            LOG(cacheLog.str());
            return true;
        }
        if (response.isExpired())
        {
            // log ID: in cache, but expired at EXPIREDTIME
            std::ostringstream cacheLog;
            cacheLog << requestId << ": in cache, but expired at " << response.getExpiredTime() << std::endl;
            LOG(cacheLog.str());
            return true;
        }
    }
    return false;
}

std::string Cache::generateValidateRequest(std::string url, Request &request)
{
    std::string validateRequest = request.getData().data();
    if (cache_map.find(url) != cache_map.end())
    {
        Response response = cache_map[url];
        // find first line
        int first_line_end = validateRequest.find("\r\n");
        // add if none match to header
        if (response.getETag() != "")
        {
            validateRequest.insert(first_line_end + 2, "If-None-Match: " + response.getETag() + "\r\n");
        }

        if (response.getLastModified() != "")
        {
            validateRequest.insert(first_line_end + 2, "If-Modified-Since: " + response.getLastModified() + "\r\n");
        }

        // std::string cacheControl = response.getCacheControl();
        // must-revalidate or max-age=0 or expired
    }
    return validateRequest;
}
