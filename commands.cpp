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
    //if (token == "QUIT")
    //    quit(i); //! clientarrayden + channelclientstan silinecek (erase())
    if (!token1.empty() && !token2.empty())
    {
        if (token == "JOIN")
            join(i, token1, token2);
        /*
        else if (token == "PRIVMSG")
            mess(i, token1, token2);
        else if (token == "KICK")
            kick(i, token1, token2);
        else if (token == "TOPIC")
        {
            std::string temp;
            while (iss >> temp) {
                token2 += temp; token2 += " ";
            }
            topic(i, token1, token2);
        }
        */
    }
    /*
    else if (token == "PART")
        part(i, token1);
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


void Server::join(int i, std::string channelName, std::string channelPass)
{
    if (channelName[0] != '#') {
        sendFunct(clientArray[i].getSocketFd(), "Channel name should start with '#'\r\n");
        return;
    }
    std::string mess;
    if((int)channelArray.size() == 0) {
        channelArray.push_back(Channel(channelName, channelPass, clientArray[i])); // ***
    //    channelArray[0].addOperator(whichClient);
        mess = "JOIN You are now in channel "  + channelName +" \r\n";

    }
    else
    {
        int controlFlag1 = -1;
        int controlFlag2 = -1;
        for (int ch = 0; ch < (int)channelArray.size(); ch++)
        {
            if(!channelArray[ch].getName().compare(channelName))
            {
                for (int k = 0; k < (int)channelArray[ch].getClientArray().size(); k++) {
                    if (!channelArray[ch].getClientArray()[k].getNickname().compare(clientArray[i].getNickname()))
                    {
                        controlFlag1 = 1;
                        controlFlag2 = 1;
                        mess = "JOIN You are already in channel named "  + channelName +" !\r\n";
                        return;
                    }
                }
                if(controlFlag1 == -1) {
                    if((!channelArray[ch].getPassword().compare(channelPass)))
                    { 
                        channelArray[ch].addClientArray(clientArray[i]);
                        mess = "JOIN You are now in channel "  + channelName +" \r\n";
                    }
                    else
                        mess = BADCHANNEL(channelName);
                    controlFlag2 = 1;
                }
                break;
            }
        }
        if(controlFlag2 == -1) {
            channelArray.push_back(Channel(channelName, channelPass, clientArray[i]));
        //    channelArray[(int)channelArray.size() - 1].addOperator(whichClient);
            mess = "JOIN You are now in channel "  + channelName +" \r\n";
        }
    }
    std::string buffer = ":" + clientArray[i].getNickname() + "!" + clientArray[i].getUsername() + "@" + getHostname() + ": "  +  mess + "\r\n";;
    sendFunct(clientArray[i].getSocketFd(),buffer);
}

void Server::part(int i, std::string channelName)
{
    (void)channelName;
    (void)i;
}


//! 