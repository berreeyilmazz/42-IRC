#include "Server.hpp"

int main(int ac, char const** av)
{
    if (ac != 3) {
        std::cout << "The executable will be run as follows: ./ircserv <port> <password>" << std::endl;
        return (1);}

    Server server;

    if (checkAv(av, server)) {
        std::cout << "Wrong port" << std::endl;
        return (1);  }
    std::cout  << server.getPort() << " , "  << server.getPassword() << " , " << server.getHostname() << std::endl;
    
    
    return 0;
}




c++ -std=c++98 main.cpp Server.cpp login.cpp Socket.cpp