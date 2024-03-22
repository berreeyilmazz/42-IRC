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
Channel::Channel(std::string name, std::string password, Client &client)
{
    this->_name = name;
    this->_password = password;
    channelClients.push_back(client);
}

void Channel::setTopic(std::string topic) 
{
    this->_topic = topic;
}

std::string Channel::getTopic() {
    return (_topic);
}

void Channel::addOperator(int index) 
{
    _operator.push_back(int());
    _operator[_operator.size() - 1] = index;
}

std::vector<int> Channel::getOperator() 
{
    return (this->_operator);
}

Channel::Channel()
{
    
}

Channel::~Channel()
{

}


void    Channel::eraseOperator(int op) {
    channelClients.erase(channelClients.begin() + op);
}


// compare 1 dönüyosa aynılar
int Channel::areYouOperator(std::string nick) {
   //!!!!!!!
   (void) nick;
   return (0);
}

std::vector<Client> Channel::getClientArray()
{
    return(channelClients);
}


void Channel::addClientArray(Client &ref) {
    channelClients.push_back(Client());
    channelClients[(int)channelClients.size() - 1].setSocketFd(ref.getSocketFd());
    channelClients[(int)channelClients.size() - 1].setNickname(ref.getNickname());
}