#include "Client.hpp"

Client::Client(){
    this->status = "offline";
}

Client::~Client(){}

void Client::setNickname(std::string _nickname) {
    this->nickname = _nickname;
}

void Client::setUsername(std::string _username) {
    this->username = _username;
}

void Client::setPassword(std::string password){
    this->password = password;
}

void Client::setPersonalPass(std::string personalPass){
    this->personalPass = personalPass;
}

void Client::setStatus(std::string status){
    this->status = status;
}

void Client::setUserPass(std::string userPass){
    this->userPass = userPass;
}

std::string Client::getNickname() {
    return (this->nickname);
}

std::string Client::getUsername() {
    return (this->username);
}

std::string Client::getPassword() {

    return (this->password);
}

std::string Client::getPersonalPass() {

    return (this->personalPass);
}

std::string Client::getStatus() {

    return (this->status);
}

std::string Client::getUserPass() {

    return (this->userPass);
}

void Client::setSocketFd(int i) {
    socket_fd = i;
}

void Client::setRealName(std::string _realName) {
    this->realName = _realName;
}

std::string Client::getRealName() {
    return (this->realName);
}


void Client::setHost(std::string _host) {
    this->host = _host;
}

std::string Client::getHost() {
    return (this->host);
}

int Client::getSocketFd() {
    return(socket_fd);
}

