#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class Socket {
    private:
        int                 socket_fd;
        int                 new_socket;
        struct sockaddr_in  address;
        socklen_t           addrlen;
    public:
        bool createSocket(int _port);
};


#endif