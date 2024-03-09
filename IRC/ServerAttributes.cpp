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
    std::string token1, token2;
    iss >> token1;
    iss >> token2;
    if (clientArray[i].getStatus() == "offline") 
    {
        if (token == "USERPASS" && clientArray[i].getUserPass().empty()) { //user ilk kez giriyorsa
            if (!token1.empty()) {
                clientArray[i].setUserPass(token1);
                clientArray[i].setStatus("online");
                sendFunct(clientArray[i].getSocketFd(), LOGIN(clientArray[i].getNickname(), clientArray[i].getUsername()));
            }
        }
        else if (token == "USERPASS" && clientArray[i].getStatus() == "offline")
        {
            if (!token1.empty())
                authenticateClient(i, token1);
        }

        else if(token =="PASS") 
        {
            if (token1.compare(password)) 
                sendFunct(clientArray[i].getSocketFd(), "wrong password\r\n");
            else
                clientArray[i].setPassword(token1);
        }

        else if (token =="USER")
            clientArray[i].setUsername(token1);

        else if (token == "NICK") {
            if (isNicknameInUse(token1) !=  -1)
                clientArray[i].setNickname(token1);
            else 
                sendFunct(new_socket, NICKNAME_IN_USE(token1));
        }

        if ((clientArray[i].getUsername().empty() || clientArray[i].getNickname().empty() || clientArray[i].getPassword().empty()))
            sendFunct(clientArray[i].getSocketFd(), "You should enter the server: USER <your user>    NICK <your nick>\r\n");

        else if (!clientArray[i].getUsername().empty() && !clientArray[i].getNickname().empty() && !clientArray[i].getPassword().empty()) {

            clientArray[i].setStatus("online");
        }
    }

    else if (clientArray[i].getStatus() == "online")
    {
        if (token == "NOTICE");
        else if (token == "QUIT")
            somebodyLeft(clientArray[i]);
        else if (token == "CAP" || token == "PING");
        else if (token == "JOIN") 
        {
            if (!token1.empty() && !token2.empty())
                joinFunct(i, token1, token2);
            else
                sendFunct(clientArray[i].getSocketFd(), "missing password\r\n");
        }
        else if (token == "PART") {

            if (!token1.empty())
                partFunct(i, token1);
        }
        else if (token == "KICK") 
        {
            if (token1[0] != '#')
                token1 = "#" + token1;
            if (!token1.empty() && !token2.empty())
                kickFunct(i, token1, token2);
        }
        else if (token == "PRIVMSG")
            channelOrPrivMess(i, token1, buffer);
        else if (token == "TOPIC") 
        {
            if (!token1.empty() && token1[0] != '#')
                token1 = "#" + token1;
            std::string temp;
            while (iss >> temp) {
                token2 += temp;
                token2 += " ";
            }
            if (!token1.empty() && !token2.empty()) {
                sendTopic(i, token1, token2);
            }
        }
        else if (token == "WHO") // WHO #kanal 
            whoFunct(i, token1, token2);
        else if (token == "help") 
            sendFunct(clientArray[i].getSocketFd(), "JOIN / PART / PRIVMSG / TOPIC / KICK\r\n");
        /*
        else if (token == "MODE");  // MODE #kanal b || MODE #kanal
        */
    }

    else if (token == "help")
        sendFunct(clientArray[i].getSocketFd(), "You should enter the server: USERPASS <your userpass>\r\n");
    else
        sendFunct(clientArray[i].getSocketFd(), "You should enter the server\r\n");

}

void Server::addOnlyWithFd(int new_socket) {
    clientArray.push_back(Client());
    clientArray[clientArray.size() - 1].setSocketFd(new_socket);
    sendFunct(new_socket, "Please enter your user and nickname: USER <your user>  /  NICK <your nick>\r\n");
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
                    std::string msg =  ": 332 " + channelArray[j].channelClients[k].getNickname() + " " + channelArray[j].getName() + ":" + channelArray[j].getTopic() + "\r\n";
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
        sendFunct(clientArray[i].getSocketFd(),": 403 : " + clientArray[i].getNickname() + " " + channelName+ " :There is no channel\r\n");
}
