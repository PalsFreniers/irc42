#pragma once

#include "../includes.hpp"

class Server {
public:
        Server(int port, int max_clients);
        Server(Server const &src);
        ~Server();
        Server &operator=(Server const &src);
        void initializeServer();
        void initializePoll();
        void closeFdandSocket();
        void addClient(Client &client);
        int getSocket();
        void setSocket(int src);
        sockaddr_in &getAddress();
        void setAddress(sockaddr_in src);
        std::vector<struct pollfd> &getFds();
        void setFds(std::vector<struct pollfd> &src);
        std::vector<Client> &getClients();
private:
        int _port;
        int _max_clients;
        int _socket;
        sockaddr_in _address;
        std::vector<struct pollfd> _fds;
        std::vector<Client> _client;
};
