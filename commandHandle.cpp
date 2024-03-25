#include "Server.hpp"

void Server::join(int i, std::string channelName, std::string channelPass)
{
    int ch = whichChannel(channelName);
    if (ch == -1)
    {
        channelArray.push_back(Channel(channelName, channelPass, clientArray[i])); // ***
        channelArray[channelArray.size() - 1].addOperator(i);
        sendFunct(clientArray[i].getSocketFd(), JOIN(channelName));
        return;
    }
    std::cout << "hjkrdhr" << std::endl;
    for(int k = 0; k < (int)channelArray[ch].channelClients.size(); k++)
    {
        if (!channelArray[ch].channelClients[k].getNickname().compare(clientArray[i].getNickname())) 
        {
            sendFunct(clientArray[i].getSocketFd(), "You already in this channel\r\n");
            return;
        }
    }
    if((channelArray[ch].getPassword().compare(channelPass)) == 0)
    {
        channelArray[ch].channelClients.push_back(clientArray[i]);
        sendFunct(clientArray[i].getSocketFd(), JOIN(channelName));
        return;
    }
    sendFunct(clientArray[i].getSocketFd(), BADCHANNEL(channelName));
}

int Server::whichChannel(std::string channelName)
{
    int index = (int)channelArray.size() - 1;
    
    while (index >= 0)
    {
        if (!channelArray[index].getName().compare(channelName))
            return (index);
    }
    return (-1);
}

// clientArray[i]    ***** PARt #kanal

void    Server::part( int clientIndex, std::string token )
{
    if (token[0] != '#')
    {
        sendFunct(clientArray[clientIndex].getSocketFd(), "The channel name should start with '#' !! \r\n");
        return;
    }

    int channelIndex = whichChannel(token);

    if (channelIndex == -1)
    {
        sendFunct(clientArray[clientIndex].getSocketFd(), "Any channel not found with name " + token + "\r\n");
    }
    for (int j = 0; j < (int)channelArray[channelIndex].channelClients.size(); j++)
    {
        if (clientArray[clientIndex].getNickname().compare(channelArray[channelIndex].channelClients[j].getNickname())  == 0)
            channelArray[channelIndex].channelClients.erase(channelArray[channelIndex].channelClients.begin() + j);
    }

}

void Server::quit(int i)
{
    sendFunct(clientArray[i].getSocketFd(), "BYE!\r\n");
    clientArray.erase(clientArray.begin() + i);
    for (int ch = 0; ch < (int)channelArray.size(); ch++)
    {
        for (int c = 0; c < (int)channelArray[ch].channelClients.size(); c++)
        {
            if (channelArray[ch].channelClients[c].getNickname().compare(clientArray[i].getNickname()) == 0)
            {
                channelArray[ch].channelClients.erase(channelArray[ch].channelClients.begin() + c);
                if (channelArray[ch].channelClients.size() == 0){
                    channelArray.erase(channelArray.begin() + ch);
                    continue;
                }
                for (int o = 0; o < (int)channelArray[ch].getOperator().size(); o++)
                {
                    if (i == channelArray[ch].getOperator()[o])
                    {
                        channelArray[ch].eraseOperator(i);
                        if (channelArray[ch].getOperator().size() == 0)
                        {
                            if ((int)channelArray[ch].channelClients.size() >= c)
                                channelArray[ch].addOperator(findIndex(channelArray[ch].channelClients[c + 1].getNickname()));
                            else
                                channelArray[ch].addOperator(findIndex(channelArray[ch].channelClients[0].getNickname()));
                        }
                    }
                }
            }
        }
    }
    
}

int Server::findIndex(std::string nick)
{
    for(int i = 0; i< (int)clientArray.size(); i++)
    {
        if (clientArray[i].getNickname().compare(nick)== 0)
            return (i);
    }
    return (-1);
}

void    topic()
{
    
}