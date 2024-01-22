#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include "Socket.hpp"


class Server {
    private:
        int             port;
        std::string     password;
        std::string     hostname;
    public:
        Server();
        ~Server();

        void setPort(int _port);
        void setPassword(std::string _password);
        int getPort();
        std::string getPassword();
        std::string getHostname();

        void setHostname();
        bool acceptClients();

};


int checkAv(const char **av, Server &server);

#endif