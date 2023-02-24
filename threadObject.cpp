#include "threadObject.hpp"

ThreadObject::ThreadObject(std::string ip, int request_id, int client_connection_fd) : ip(ip), request_id(request_id), client_connection_fd(client_connection_fd)
{
}
ThreadObject::~ThreadObject()
{
    close(client_connection_fd);
}

// getter
std::string ThreadObject::getIp()
{
    return ip;
}
int ThreadObject::getRequestId()
{
    return request_id;
}
int ThreadObject::getClientConnectionFd()
{
    return client_connection_fd;
}
