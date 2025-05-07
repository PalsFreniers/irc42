#pragma once

#include "Client.hpp"
#include <vector>

class Server {
public:
        Server(int port, std::string pass);
        ~Server();
        void poll();
        int accept();
        void parseAndExec(std::string msg, int currentClientIndex);
private:
        int _sock;
        std::vector<Client> _clients;
        std::string _pass;
};
