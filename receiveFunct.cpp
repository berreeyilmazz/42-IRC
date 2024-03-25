#include "Server.hpp"

void Server::commands(int i) 
{
    char buffer[1028] = {0};
    valread = 0;
    if ((valread = recv(clientArray[i].getSocketFd(), buffer, sizeof(buffer) - 1, 0)) <= 0)
        return;
    buffer[valread] = '\0';
    std::cout << "+++++++" <<  buffer << "+++" <<std::endl;
    std::istringstream iss(buffer);
    std::string token, token1, token2;
    iss >> token;
    iss >> token1;
    iss >> token2;
    if (token == "QUIT")
        quit(i); //! clientarrayden + channelclientstan silinecek (erase())
    if (!token.empty() && !token1.empty() && !token2.empty())
    {
        if (token == "JOIN")
            join(i, token1, token2);
        else if (token == "PRIVMSG")
            mess(i, token1, buffer);
        else if (token == "KICK")
            kick(i, token1, token2);

        /*
        else if (token == "TOPIC") // TOPIC #kanalismi ot saticaz sattık satmadık yaptık ettik
        {
            std::string temp;
            while (iss >> temp) {
                token2 += temp;
                token2 += " ";
            }
            topic(i, token1, token2);
        }
        */
        
    }
    if (!token.empty() && !token1.empty()) 
    {
        if (token == "PART") // PART #kanalismi   bu kanaldaki nicknamelerle clientArray[i].getNickname aynı mı aynıysa sik
           part(i, token1);
    }
    /*
    else if (token == "WHO")
        who(i, token1, token2);
    else if (token == "MODE")
        mode(i, token1, token2);
    else if (token == "list")
        list(i);
    */
    else if (token == "help") 
       sendFunct(clientArray[i].getSocketFd(), "Available commands:  JOIN / PART / PRIVMSG / KICK / TOPIC\r\n");
    else
        sendFunct(clientArray[i].getSocketFd(), "You've entered missing parameters or type 'help' for available commands!");
}

