#pragma once

#include <fcntl.h>
#include "Messages.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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

        void        connectToNewClient(int n_socket);
        void        info(int i);
        void        running();
        int         isNicknameInUse(std::string _nick);
        void        commands(int i);
        void        join(int i, std::string channelName, std::string channelPass);
        void        part(int i, std::string channelName);



        std::string getHostname();
        void        setHostname(std::string _host);
        int         start(char *av1, char *av2);
        void        createSocket();

        void        newClient();
        void        addClient(int i);
        void        sendFunct(int fd, std::string raw);
/*
        int         isNicknameInUse(std::string _nick);
        void        joinFunct(int whichClient,std::string channelName, std::string channelPassword);
        void        kickFunct(int i, std::string channelName, std::string clientNick);
        void        sendTopic(int i, std::string channelName, std::string topicMessage);
        void        channelOrPrivMess(int i, std::string channelOrClient, std::string buffer);
        int         whichChannel(std::string channelName);
        int         areYouIn(int cl, int ch);
        std::vector<Client> getClientArray(void);
        void        setClientArray(std::vector<Client> clientArray);
        std::vector<Channel> getChannelArray(void);
        void        setChannelArray(std::vector<Channel> channelArray);
        std::string getPassword(void);
        void        setPassword(std::string password);
        void        partFunct(int i, std::string partName);
        void        whoFunct(int whichClient, std::string token1, std::string token2);
        void        addOnlyWithFd(int new_socket);
        void        modeFunct(int i, std::string token1, std::string token2);
        int         matchClient(int  ch, int clIndx);
*/

};

