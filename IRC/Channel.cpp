#include "Channel.hpp"

std::string Channel::getName(void)
{
    return(this->_name);
}

std::string Channel::getPassword(void)
{
    return(this->_password);
}

void Channel::setName(std::string name)
{
    this->_name = name;
}

void Channel::setPassword(std::string password)
{
    this->_password = password;
}

Channel::Channel(std::string name, std::string password)
{
    this->_name = name;
    this->_password = password;
}
Channel::Channel(std::string name, std::string password, Client &client){
    this->_name = name;
    this->_password = password;
    channelClients.push_back(client);
}

void Channel::setTopic(std::string topic) {
    this->_topic = topic;
}

std::string Channel::getTopic() {
    return (_topic);
}

void Channel::setOperator(Client &ref) 
{
    _operator.push_back(Client());
    _operator[_operator.size() - 1].setNickname(ref.getNickname());
    _operator[_operator.size() - 1].setSocketFd(ref.getSocketFd());
}

std::vector<Client> Channel::getOperator() {
    return (this->_operator);
}

Channel::Channel(){
    
}

Channel::~Channel(){}

void Channel::addOperator(int op) {
    /*
    for (int i = 0; i < (int)_operator.size(); i++) {
        if (_operator[i] == op)
        {
            return;
        }
    }
    printf("giriş\n");
    _operator.push_back(op);
    */
    (void)op;
    printf("çıkış\n");

}

void    Channel::eraseOperator(int op) {
    channelClients.erase(channelClients.begin() + op);
}


// compare 1 dönüyosa aynılar
int Channel::areYouOperator(std::string nick) {
    for (int o = 0; o < (int)getOperator().size(); o++) 
    {
        if (_operator[o].getNickname().compare(nick))
            return (o);
    }
    return (-1);
}