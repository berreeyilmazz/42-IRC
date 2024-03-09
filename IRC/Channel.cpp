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

void Channel::setOperator(int operatorFd) {
    this->_operator = operatorFd;
}

int Channel::getOperator() {
    return (this->_operator);
}

Channel::Channel(){
    
}



Channel::~Channel(){}