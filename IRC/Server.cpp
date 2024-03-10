#include "Server.hpp"
#include "Messages.hpp"

int Server::start(char *av1, char *av2) {
    port = atoi(av1);
    if (!(port >= 0 && port <= 65535)) // 0 değişebilir
        return (1);
    
    this->password = av2;

    char str_h[256];
    gethostname(str_h, sizeof(str_h));
    std::string _h(str_h);
    setHostname(_h);
    return (0);
}

void Server::sendFunct(int fd, std::string raw) {

    const char *mess = raw.c_str();
    send(fd, mess, strlen(mess), 0);
}

void    Server::createSocket() {
    addrlen = sizeof(address);
    opt = 1;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE); 
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        close(server_fd);
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(server_fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "ERROR getting socket flags" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "ERROR setting socket to non-blocking mode" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket set to non-blocking mode successfully" << std::endl;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}


void Server::newClient() {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

void Server::detectIfTheyAreNew(std::string nick, std::string user, std::string pass) {
    int whichClient = isNicknameInUse(nick);

    if (whichClient == -1 || whichClient == -2) { 
        addClient(nick, user, pass, new_socket);
    }
    else {
        if (clientArray[whichClient].getStatus() == "online")
            sendFunct(new_socket, NICKNAME_IN_USE(clientArray[whichClient].getNickname()));
        else {
            printf("which_client %d   ,new_socket: %d\n",whichClient, clientArray[whichClient].getSocketFd());

            clientArray[whichClient].setSocketFd(new_socket);
            sendFunct(new_socket, " ::  : Please enter your password... /USERPASS <your userpass>\r\n");
        }
    }
}

void Server::addClient(std::string nick, std::string user, std::string pass, int fd) {
    clientArray.push_back(Client());
    clientArray[(int)clientArray.size() - 1].setNickname(nick);
    clientArray[(int)clientArray.size() - 1].setUsername(user);
    clientArray[(int)clientArray.size() - 1].setPassword(pass);
    clientArray[(int)clientArray.size() - 1].setSocketFd(fd);
    sendFunct(clientArray[(int)clientArray.size() - 1].getSocketFd(), " ::  : Please enter your password... /USERPASS <your userpass>\r\n");
}

int Server::isNicknameInUse(std::string _nick) {
    if (this->clientArray.size() == 0 )
        return (-1);
    for (int i = 0; i < (int)clientArray.size(); i++) 
    {
        if (!clientArray[i].getNickname().compare(_nick))
            return (i);
    }
    return (-2);
}

void Server::authenticateClient(int whichClient, std::string userpass) 
{
    if (clientArray[whichClient].getUserPass().compare(userpass)) {
        clientArray[whichClient].setSocketFd(new_socket);
        activateChannels(whichClient);
        clientArray[whichClient].setStatus("online");
    //    buraya opeator listesindeysek addOperator çağrılacak
        sendFunct(clientArray[whichClient].getSocketFd(), LOGIN(clientArray[whichClient].getNickname(), clientArray[whichClient].getUsername()));
    }
    else
        sendFunct(clientArray[whichClient].getSocketFd(),": : wrong password\r\n");


}

void Server::activateChannels(int whichClient) {
    for (int ch = 0; ch < (int)channelArray.size(); ch++) {
        if (areYouIn(whichClient, ch) == 1)
        {
            std::string mess = ":" + clientArray[whichClient].getNickname() + "!" + clientArray[whichClient].getUsername() + "@" 
                    + getHostname() + ": "  +  "JOIN You are now in channel "  + channelArray[ch].getName() +" \r\n";
            sendFunct(clientArray[whichClient].getSocketFd(), mess);
        }
    }
}

int Server::somebodyLeft(int i) 
{
    for (int ch = 0; ch < (int)channelArray.size(); ch++) 
    
        for (int op = 0; op < (int)channelArray[ch].getOperator().size(); op++)
        {
            if (channelArray[ch].getOperator()[op] != -1)
            {
                int sze = (int)channelArray[ch].getOperator().size();
                if (sze == 1)
                {
                    channelArray[ch].addOperator(i + 1);
                }
            }    
        }


    clientArray[i].setStatus("offline");
    close(clientArray[i].getSocketFd());
    clientArray[i].setSocketFd(0);
    return 1;
}
