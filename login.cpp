#include "Server.hpp"

int checkAv(const char **av, Server &server) {
    int port = atoi(av[1]);
    if (port >= 1024 && port <= 65535)
    {
        server.setPort(port);
        server.setPassword(av[2]);
        server.setHostname();
        return (0);
    }
    return (1);
}
