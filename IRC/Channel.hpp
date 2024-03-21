#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>

class Client;

class Channel{
    private:
        std::string             _name;
        std::string             _password;
        std::string             _topic;
        std::vector<Client>     _operator;

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

        void setOperator(Client &ref);
        std::vector<Client>   getOperator();
        void    eraseOperator(int op);
        int     areYouOperator(std::string nick);
        void    addOperator(int op);


};

#endif