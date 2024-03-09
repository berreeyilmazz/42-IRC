#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#include <iostream>

int main(int ac, char **av) {

    if (ac != 3)
        return (err("Not enough parameters!"));

    Server server;
    if (server.start(av[1], av[2]))
        return (err("Wrong port!"));
    server.createSocket();
    isSet(server);
}