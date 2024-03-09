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
            if (serverIsSet(server) == 1)
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

int serverIsSet(Server &server) {
    std::string pass, nick, user;
    while (true) {
        std::string token;
        char buffer[1028] = {0};
        server.setValRead(recv(server.getNewSocket(), buffer, sizeof(buffer) - 1, 0));
        if (server.getValRead() < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN){
                perror("recv error");
                break;
            }
        }
        else if (server.getValRead() == 0) 
        {
            printf("Istemci %d ayrildi.\n",server.getNewSocket());
            close(server.getNewSocket());
            server.setNewSocket(0);
            break;
        }
        else 
        {
            buffer[server.getValRead()] = '\0';
            std::istringstream iss(buffer);
            std::cout << "----------\n" << buffer << "----------" << std::endl;
            while (iss >> token) {
                if (token == "NICK")
                    iss >> nick;
                else if (token == "PASS") {
                    iss >> pass;
                    if (pass.compare(server.getPassword())) 
                        server.sendFunct(server.getNewSocket(), ERR_PASSWDMISMATCH(nick));
                    return (1);
                }
                else if (token == "USER")
                    iss >> user;
            }
            if (pass.empty() == 0 && nick.empty() == 0 && user.empty() == 0) {
                server.detectIfTheyAreNew(nick, user, pass);
                break;
            }
        }
    }
    return (0);
}