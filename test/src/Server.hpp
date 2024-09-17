#pragma once

#include <netinet/in.h>
#include <string>
#include <vector>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <poll.h>
#include "sha256.hpp"

class Server {
public:
        Server();
        ~Server();
        Server(const Server &other);
        Server(int port, std::string pass);
        Server &operator=(const Server &other);

        void launch();
        void accept();
        void run();
        void disconnectClient(int fd);
        void onClientMessage(int fd);
        bool isCorrectPass(std::string pass);
private:
        u32 _pass[8];
        int _port;
        int _sockfd;
        std::vector<struct pollfd> _pfds;
        sockaddr_in _addr;
        socklen_t _addrLen;
};
