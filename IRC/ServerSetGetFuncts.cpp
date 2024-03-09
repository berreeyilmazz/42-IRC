#include "Client.hpp"
#include "Channel.hpp"
#include "Messages.hpp"
#include "Server.hpp"

std::string Server::getPassword(void){
    return this->password;
}
void Server::setPassword(std::string password){
    this->password = password;
}
int Server::getPort(void){
    return this->port;
}
void Server::setPort(int port){
    this->port = port;
}
int Server::getServerFd(void){
    return this->server_fd;
}
void Server::setServerFd(int server_fd){
    this->server_fd = server_fd;
}
int Server::getNewSocket(void){
    return this->new_socket;
}
void Server::setNewSocket(int new_socket){
    this->new_socket = new_socket;
}
int Server::getActivity(void){
    return this->activity;
}
void Server::setActivity(int activity){
    this->activity = activity;
}
int Server::getMaxSd(void){
    return this->max_sd;
}
void Server::setMaxSd(int max_sd){
    this->max_sd = max_sd;
}
int Server::getValRead(void){
    return this->valread;
}
void Server::setValRead(int valread){
    this->valread = valread;
}
int Server::getOpt(void){
    return this->opt;
}
void Server::setOpt(int opt){
    this->opt = opt;
}
socklen_t Server::getAddrLen(void){
    return this->addrlen;
}
void Server::setAddrLen(socklen_t addrlen){
    this->addrlen = addrlen;
}
struct sockaddr_in Server::getAddress(void){
    return this->address;
}
void Server::setAddress(struct sockaddr_in address){
    this->address = address;
}
std::vector<Client> Server::getClientArray(void){
    return this->clientArray;
}
void Server::setClientArray(std::vector<Client> clientArray){
    this->clientArray = clientArray;
}
std::vector<Channel> Server::getChannelArray(void){
    return this->channelArray;
}
void Server::setChannelArray(std::vector<Channel> channelArray){
    this->channelArray = channelArray;
}

void    Server::setHostname(std::string _host) {
    this->hostname = _host;
}

std::string Server::getHostname() {
    return (this->hostname);
}
