#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>

class Client;

class Channel{
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        int         _operator;
    public:
        Channel(std::string name, std::string password, Client &client);
        Channel(std::string name, std::string password);

        Channel();
        ~Channel();
        std::vector<Client> channelClients;
        std::string getName(void);
        std::string getPassword(void);
        void setName(std::string name);
        void setPassword(std::string password);
        void setTopic(std::string topic);
        std::string getTopic();

        void    setOperator(int operatorFd);
        int     getOperator();
};

#endif