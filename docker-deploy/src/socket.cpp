
#include "socket.hpp"

// create a server socket and return the socket file descriptor, listen for client connection
int init_server(std::string port)
{
    int status, socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;

    status = getaddrinfo(hostname, port.c_str(), &host_info, &host_info_list);
    if (status != 0)
    {
        std::cout << "Error: cannot get address info for host " << hostname << std::endl;
        return -1;
    }

    socket_fd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        std::cout << "Error: cannot create socket" << std::endl;
        return -1;
    }

    int opt = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cout << "Error: cannot bind socket" << std::endl;
        return -1;
    }

    status = listen(socket_fd, 100);
    if (status == -1)
    {
        std::cout << "Error: cannot listen on socket" << std::endl;
        return -1;
    }
    freeaddrinfo(host_info_list);
    return socket_fd;
}
// connect to a server and return the socket file descriptor
int init_client(std::string hostname, std::string port)
{
    int status, socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname.c_str(), port.c_str(), &host_info, &host_info_list);
    if (status != 0)
    {
        std::cout << "Error: cannot get address info for host " << hostname << std::endl;
        return -1;
    }

    socket_fd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        std::cout << "Error: cannot create socket" << std::endl;
        return -1;
    }

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cout << "Error: cannot connect to socket" << std::endl;
        return -1;
    }
    freeaddrinfo(host_info_list);

    return socket_fd;
}

// listen for a connection and return the socket file descriptor
int server_accept(int socket_fd, std::string *ip)
{
    return 0;
}

// send data to a socket
void send_data(int socket_fd, std::vector<char> data, size_t data_len)
{
    int total = 0; // how many bytes we've sent

    int n;
    if (data_len > INT_MAX)
    {
        throw std::overflow_error("data is larger than INT_MAX");
    }
    int convertData = static_cast<int>(data_len);
    int len = convertData;
    int bytesleft = len; // how many we have left to send

    while (total < len)
    {
        n = send(socket_fd, data.data() + total, bytesleft, 0);
        if (n == -1)
        {
            std::cerr << "Error:: " << errno << std::endl;
            perror("Error: cannot send data");
            throw std::exception();
            // break;
        }
        total += n;
        bytesleft -= n;
    }

    // len = total; // return number actually sent here

    // return n == -1 ? -1 : 0; // return -1 on failure, 0 on success

    // int recvLen = send(socket_fd, data.data(), data_len, 0);
    // if (recvLen == -1)
    // {
    //     // std::cout << "Error: cannot send data" << std::endl;
    //     std::cerr << "Error:: " << errno << std::endl;
    //     perror("Error: cannot send data");
    //     throw std::exception();
    // }
    // std::cout << "send data: " << data << std::endl;
    // return recvLen;
}

// receive data from a socket
int recv_data(int socket_fd, std::vector<char> &req_msg)
{
    // std::vector<char> req_msg(BUF_LEN);
    // print socket_fd
    // std::cout << "socket_fd: " << socket_fd << std::endl;
    ssize_t recvLen = recv(socket_fd, &req_msg.data()[0], 65536, 0);

    if (recvLen == -1)
    {
        // std::cout << "Error: cannot receive data" << std::endl;
        std::cerr << "Error:: " << strerror(errno) << std::endl;
        perror("Error: cannot receive data");
        throw std::exception();
    }
    return recvLen;
}
