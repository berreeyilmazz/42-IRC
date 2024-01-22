#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::setPort(int _port) {
    this->port = _port;
}

void Server::setPassword(std::string _password) {
    this->password = _password;
}

int Server::getPort() { return(this->port); }

std::string Server::getPassword() { return(this->password); }

void Server::setHostname() {
    char _hostname[1024];
    if (gethostname(_hostname, 1024) < 0)
        exit(1);
    this->hostname = _hostname;
}

std::string Server::getHostname() { return (this->hostname); }

bool Server::acceptClients() {
    Socket  socket;
    socket.createSocket(port);

/*
    POLL KULLANILACAK OKUMA İŞLEMİ İÇİN FLN "POLLIN" kullanmayı araştırmak lazım

    https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select

    if ((new_socket = accept(serverSocketserver_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    ssize_t valread;
    char buffer[1024] = { 0 };
    const char *hello = "/msg NICKSERV USER 123\0";
    int flag = 0;
    if (send(new_socket,hello, strlen(hello), 0) < 0) {
        perror("Hata: Komut gönderilemedi");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    std::cout << "welcome gönderildi" << std::endl;
    char buff[1024] = {0};
    while (true) {
        char buff[1024];
        int bytesRead = recv(new_socket, buff, sizeof(buff) - 1, 0);
        if (bytesRead > 0) {
        buff[bytesRead] = '\0';
     // Sunucudan gelen mesajı ekrana yazdırma
        std::cout << "Sunucu Cevabı: " << buff << std::endl;

        // Numerik kodu kontrol etme
        if (strstr(buff, "001") != nullptr) {
            // 001 numerik kodu için özel işlemleri gerçekleştirme
        std::cout << "Bağlantı başarıyla sağlandı!" << std::endl;
        }
        }
    }
    //close


*/
return (0);
}

