#include "Server.hpp"

void        Server::setHostname(std::string _host)
{
    this->hostname = _host;
}

std::string Server::getHostname()
{
    return (hostname);
}

int Server::isNicknameInUse(std::string _nick) {
    for (int i = 0; i < (int)clientArray.size(); i++) 
    {
        if (!clientArray[i].getNickname().compare(_nick))
            return (1);
    }
    return (0);
}


void Server::sendFunct(int fd, std::string raw) 
{
    const char *mess = raw.c_str();
    send(fd, mess, strlen(mess), 0);
}
