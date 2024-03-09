#include "Client.hpp"
#include "Channel.hpp"
#include "Messages.hpp"
#include "Server.hpp"

void Server::joinFunct(int whichClient,std::string channelName, std::string channelPassword) { 
    std::string mess;
    if((int)channelArray.size() == 0) {
        channelArray.push_back(Channel(channelName, channelPassword, clientArray[whichClient])); // ***
        channelArray[0].setOperator(clientArray[whichClient].getSocketFd());
        mess = "JOIN You are now in channel "  + channelName +" \r\n";

    }
    else {
        int controlFlag1 = -1;
        int controlFlag2 = -1;
        for (int i = 0; i < (int)channelArray.size(); i++)
        {
            if(!channelArray[i].getName().compare(channelName))
            {
                for (int k = 0; k < (int)channelArray[i].channelClients.size(); k++) {
                    if (!channelArray[i].channelClients[k].getNickname().compare(clientArray[whichClient].getNickname()))
                    {
                        controlFlag1 = 1;
                        controlFlag2 = 1;
                        mess = "JOIN You are now in channel "  + channelName +" \r\n";
                        return;
                    }
                }
                if(controlFlag1 == -1) {
                    if((!channelArray[i].getPassword().compare(channelPassword)))
                    { 
                        channelArray[i].channelClients.push_back(clientArray[whichClient]);
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
            channelArray.push_back(Channel(channelName, channelPassword, clientArray[whichClient]));
            channelArray[(int)channelArray.size() - 1].setOperator(clientArray[whichClient].getSocketFd());
            mess = "JOIN You are now in channel "  + channelName +" \r\n";
        }
    }
    std::string buffer = ":" + clientArray[whichClient].getNickname() + "!" + clientArray[whichClient].getUsername() + "@" + getHostname() + ": "  +  mess + "\r\n";;
    sendFunct(clientArray[whichClient].getSocketFd(),buffer);
}



void Server::partFunct(int i, std::string partName) {
    int iter;
    int iter2;
    int cf3 = 0;
    std::cout << "partName: " << partName << std::endl;
    for(int i = 0; i < (int)channelArray.size(); i++)
    {
        if(channelArray[i].getName() == partName)
        {
            cf3 = 1;
            iter = i;
            break;    
        }
        cf3 = 0;
    }
    if(cf3)
    {
        int cf4 = 0;
        for(int k = 0; k < (int)channelArray[iter].channelClients.size(); k++)
        {
            if(!channelArray[iter].channelClients[k].getNickname().compare(clientArray[i].getNickname()))
            {
                cf4 = 1;
                iter2 = k;
                break;
            }
            cf4 = 0;
        }
        if(cf4)
        {
            std::string buffer = ":" + clientArray[i].getNickname() + "!" + clientArray[i].getUsername() + "@" + getHostname() + " : PART  You're not on that channel\r\n";
            sendFunct(clientArray[i].getSocketFd(), buffer);
            channelArray[iter].channelClients.erase(channelArray[iter].channelClients.begin() + iter2);
            if(channelArray[iter].channelClients.size() <= 0) {
                channelArray.erase(channelArray.begin() + iter);
            }
        }
        else

            sendFunct(clientArray[i].getSocketFd(), ": 442 " + clientArray[i].getNickname() + " :You're not on that channel\r\n");
    }
    else
        sendFunct(clientArray[i].getSocketFd(),": 403 " + partName + " :There is no channel!\r\n");

}

void Server::channelOrPrivMess(int p, std::string channelOrClient, std::string buffer) {
    if (channelOrClient[0] == '#') 
    {
        int whichChannel = this->whichChannel(channelOrClient);
        if (whichChannel == -1)
            return;
        else {  // :sender_nick!sender_user@k1m02s04.42istanbul.com.tr: PRIVMSG #kanal :aaa bbb cccc
                std::string identity = ":" + clientArray[p].getNickname() + "!" + clientArray[p].getUsername() + "@" + getHostname() + ": ";
	            std::string message = identity + buffer + "\r\n";
                for (int i = 0; i < (int)channelArray[whichChannel].channelClients.size(); i++)
                {
                    if (areYouIn(p, whichChannel) == 1) {

                        if (channelArray[whichChannel].channelClients[i].getSocketFd() != clientArray[p].getSocketFd() && channelArray[whichChannel].channelClients[i].getSocketFd() != 0)
                            sendFunct(channelArray[whichChannel].channelClients[i].getSocketFd(), message);
                    }
                }
        }
    }
    else {    // :berre!aa@k1m02s04.42istanbul.com.tr: PRIVMSG berre :hello bro
        for (int c = 0; c < (int)clientArray.size(); c++) {
            if (!clientArray[c].getNickname().compare(channelOrClient)) {
   
                std::string identity = ":" + clientArray[p].getNickname() + "!" + clientArray[p].getUsername() + "@" + getHostname() + ": ";
                std::string mess = identity + buffer + "\r\n";
                sendFunct(clientArray[c].getSocketFd(),mess);
            }
        }
    }

}

void    Server::kickFunct(int i, std::string channelName, std::string clientNick) {
// kicklenen biri operatörse
    for (int j = 0; j < (int)channelArray.size(); j++) 
    {
        if (channelName == channelArray[j].getName()) 
        {
            for (int c = 0; c < (int)channelArray[j].channelClients.size(); c++) 
            {
                if ((clientNick == channelArray[j].channelClients[c].getNickname()) && (channelArray[j].getOperator() == clientArray[i].getSocketFd())) 
                {
                    std::string kickerMessage = ": 441 " + clientArray[i].getNickname() + " : you kicked " + clientNick + "\r\n";
                    std::string kickedMessage = ": 441 " + clientNick + " : you are kicked from " + channelArray[j].getName() + "\r\n";
                    sendFunct(clientArray[i].getSocketFd(), kickerMessage);
                    sendFunct(channelArray[j].channelClients[c].getSocketFd(), kickedMessage);
                    channelArray[j].channelClients.erase(channelArray[j].channelClients.begin() + c);
                    return;
                }
            }
        }
    }
    sendFunct(clientArray[i].getSocketFd(), "No such channel or user!\r\n");

}

void Server::whoFunct(int whichClient, std::string token1, std::string token2) 
{
    int ch = whichChannel(token1);

    if(ch == -1)
        sendFunct(clientArray[whichClient].getSocketFd(), "there is no such channel");

    else if (token2.empty() == 0)
    {
        for (int cl = 0; cl < (int)channelArray[ch].channelClients.size(); cl++)
        {
            sendFunct(clientArray[whichClient].getSocketFd(), "Nick: " + channelArray[ch].channelClients[cl].getNickname() + "\n");
        }
        sendFunct(clientArray[whichClient].getSocketFd(), (": 315 " +  clientArray[whichClient].getNickname() + ": End of WHO list\r\n"));
        return ;
    }
    else
    {
        for (int cl = 0; cl < (int)channelArray[ch].channelClients.size(); cl++)
        {
            if(token2.compare(channelArray[ch].channelClients[cl].getNickname()) == 0)
            {
                sendFunct(clientArray[whichClient].getSocketFd(), "Nick: " + channelArray[ch].channelClients[cl].getNickname() + "\n");
                sendFunct(clientArray[whichClient].getSocketFd(), "Username: " + channelArray[ch].channelClients[cl].getUsername() + "\n");
                break ;
            }
        }
    }
    sendFunct(clientArray[whichClient].getSocketFd(), (": 315 " +  clientArray[whichClient].getNickname() + ": End of WHO list\r\n"));
}