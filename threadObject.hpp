#ifndef threadObject_hpp
#define threadObject_hpp

#include <string>
#include <sys/socket.h>
#include <unistd.h>

class ThreadObject
{
public:
    ThreadObject(std::string ip, int request_id, int client_connection_fd);
    ~ThreadObject();

    // getter
    std::string getIp();
    int getRequestId();
    int getClientConnectionFd();

private:
    std::string ip;
    int request_id;
    int client_connection_fd;
};
#endif /* threadObject_hpp */