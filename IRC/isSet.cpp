#include "Server.hpp"
#include "Messages.hpp"

int isSet(Server &server)
{
     while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server.getServerFd(), &readfds);

        server.setMaxSd(server.getServerFd());
        for (int i = 0; i < (int)server.getClientArray().size(); i++) {
            int clientSocket = server.getClientArray()[i].getSocketFd();
            if (clientSocket > 0) {
                FD_SET(clientSocket, &readfds);
                server.setMaxSd(std::max(server.getMaxSd(), clientSocket));
            }
        }
        struct timeval timeout;
        timeout.tv_sec = 25;
        timeout.tv_usec = 0;
        server.setActivity(select(server.getMaxSd() + 1, &readfds, NULL, NULL, &timeout));
        if ((server.getActivity() < 0) && (errno != EINTR))
            perror("select error");
        if (FD_ISSET(server.getServerFd(), &readfds)) {
            server.newClient();
            server.addOnlyWithFd(server.getNewSocket());
        }
        for (int i = 0; i < (int)server.getClientArray().size(); i++){
            if (FD_ISSET(server.getClientArray()[i].getSocketFd(), &readfds)){
                server.clientIsset(i);
            }
        }
    }
    return (0);
}