 #ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>

#include <string>
#include <iostream>
#include <fcntl.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Messages.hpp"

class Server {
    private:
        std::string hostname;
        std::string password;
        int         port;
        int         server_fd;
        int         new_socket;
        int         activity;
        int         max_sd;
        int         valread;
        int         opt;
        socklen_t addrlen;
        struct sockaddr_in address;

        std::vector<Client> clientArray;
        std::vector<Channel> channelArray;
    public:
        void        setHostname(std::string _host);
        std::string getHostname();
        int         start(char *av1, char *av2);
        void        printPort();
        void        createSocket();
        int         somebodyLeft(Client &client);
        void        newClient();
        void        detectIfTheyAreNew(std::string nick, std::string user, std::string pass);
        int         isNicknameInUse(std::string _nick);
        void        addClient(std::string nick, std::string user, std::string pass, int fd);
        void        sendFunct(int fd, std::string raw);
        void        authenticateClient(int whichClient, std::string userpass);
        void        clientIsset(int i);
        void        joinFunct(int whichClient,std::string channelName, std::string channelPassword);
        void        kickFunct(int i, std::string channelName, std::string clientNick);
        void        sendTopic(int i, std::string channelName, std::string topicMessage);
        void        channelOrPrivMess(int i, std::string channelOrClient, std::string buffer);
        int         whichChannel(std::string channelName);
        int         areYouIn(int cl, int ch);
        int         getPort(void);
        void        setPort(int port);
        int         getServerFd(void);
        void        setServerFd(int server_fd);
        int         getNewSocket(void);
        void        setNewSocket(int new_socket);
        int         getActivity(void);
        void        setActivity(int activity);
        int         getMaxSd(void);
        void        setMaxSd(int max_sd);
        int         getValRead(void);
        void        setValRead(int valread);
        int         getOpt(void);
        void        setOpt(int opt);
        socklen_t   getAddrLen(void);
        void        setAddrLen(socklen_t addrlen);
        struct sockaddr_in getAddress(void);
        void        setAddress(struct sockaddr_in address);
        std::vector<Client> getClientArray(void);
        void        setClientArray(std::vector<Client> clientArray);
        std::vector<Channel> getChannelArray(void);
        void        setChannelArray(std::vector<Channel> channelArray);
        std::string getPassword(void);
        void        setPassword(std::string password);
        void        partFunct(int i, std::string partName);
        void        activateChannels(int whichClient); 
};

int serverIsSet(Server &server);
int isSet(Server &server);

#endif