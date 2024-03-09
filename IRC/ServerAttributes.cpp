#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::clientIsset(int i) {
    char buffer[1028] = {0};
    valread = 0;
    if ((valread = recv(clientArray[i].getSocketFd(), buffer, sizeof(buffer) - 1, 0)) <= 0){
        close(clientArray[i].getSocketFd()); //close yerine operatorü de denetlediğimiz somebodyLeft çağrılacak
        clientArray[i].setSocketFd(0);
        clientArray[i].setStatus("offline");
        return;
    }
    buffer[valread] = '\0';
    std::cout << "==============" <<  buffer << "==============" <<std::endl;
    std::string  token;
    std::istringstream iss(buffer);
    
    iss >> token;
    
    if (token == "USERPASS" && clientArray[i].getUserPass().empty()) { //user ilk kez giriyorsa
        std::string userpass;
        iss >> userpass;
        if (!userpass.empty()) {
            clientArray[i].setUserPass(userpass);
            clientArray[i].setStatus("online");
            sendFunct(clientArray[i].getSocketFd(), LOGIN(clientArray[i].getNickname(), clientArray[i].getUsername()));
        }
    }
    else if (token == "USERPASS" && clientArray[i].getStatus() == "offline") { // authenticationü
        std::string userpass;
        iss >> userpass;
        if (!userpass.empty())
            authenticateClient(i, userpass);
    }
    else if (clientArray[i].getStatus() == "online")
    {
        if (token == "NOTICE");
        else if (token == "QUIT")
         {
            clientArray[i].setStatus("offline");
            close(clientArray[i].getSocketFd());
            if (new_socket != 0)
                close(new_socket);
            clientArray[i].setSocketFd(0);
            //operator
        }
        else if (token == "CAP" || token == "PING");
        else if (token == "JOIN") {
            std::string channelName, channelPassword;
            iss >> channelName;
            iss >> channelPassword;
            if (!channelName.empty() && !channelPassword.empty())
                joinFunct(i, channelName, channelPassword);
            else
                sendFunct(clientArray[i].getSocketFd(), "missing password\r\n"); // bu da böyle bisi
        }

        else if (token == "PART") {
            std::string partName;
            iss >> partName;
            partFunct(i, partName);
        }
        else if (token == "KICK") {
            std::string channelName, clientNick;
            iss >> channelName;
            if (channelName[0] != '#')
                channelName = "#" + channelName;
            iss >> clientNick;
            if (!channelName.empty() && !clientNick.empty())
                kickFunct(i, channelName, clientNick);

        }
        else if (token == "PRIVMSG") {
            std::string channelOrClient;
            iss >> channelOrClient;
            channelOrPrivMess(i, channelOrClient, buffer);
        }

        else if (token == "TOPIC") {
            std::string temp, topicMessage, channelName;
            iss >> channelName;
            if (!channelName.empty() && channelName[0] != '#')
                channelName = "#" + channelName;
            while (iss >> temp) {
                topicMessage += temp;
                topicMessage += " ";
            }
            if (!topicMessage.empty() && !channelName.empty()) {
                sendTopic(i, channelName, topicMessage);
            }
        }
        /*
        else if (token == "WHO"); // WHO #kanal
        else if (token == "MODE");  // MODE #kanal b || MODE #kanal
        else
            sendFunct(clientArray[i].getSocketFd(), "You should enter the server\r\n");
        */
    }
}


int Server::areYouIn(int cl, int ch) {
    for (int i = 0; i < (int)channelArray[ch].channelClients.size(); i++)
    {
        if (!channelArray[ch].channelClients[i].getNickname().compare(clientArray[cl].getNickname()))
            return (1);
    }
    return (0);
}

int Server::whichChannel(std::string channelName) {
    for (int i = 0; i < (int)channelArray.size(); i++) 
    {
        if (channelArray[i].getName() == channelName) {
            return (i);
        }
    }
    return (-1);
}

void Server::sendTopic(int i, std::string channelName, std::string topicMessage) {
    int flag = 0;
     for (int j = 0; j < (int)channelArray.size(); j++) 
    {
        if (channelName == channelArray[j].getName()) 
        {
            flag = 1;
            if (clientArray[i].getSocketFd() == channelArray[j].getOperator())
            {
                channelArray[j].setTopic(topicMessage);
                for (int k = 0; k < (int)channelArray[j].channelClients.size(); k++) {
                    std::string msg =  ": 332 " + channelArray[j].channelClients[k].getNickname() + " " + channelArray[j].getName() + " : " + channelArray[j].getTopic() + "\r\n";
                    sendFunct(channelArray[j].channelClients[k].getSocketFd(), msg);
                }
                return;
            }
            else
                sendFunct(clientArray[i].getSocketFd(), ": 403 : " +  clientArray[i].getNickname() + "you are not operator!\r\n");
            break;
        }
    }
    if (flag == 0)
        sendFunct(clientArray[i].getSocketFd(),": 403 : " + clientArray[i].getNickname() + " There is no channel\r\n");
}
