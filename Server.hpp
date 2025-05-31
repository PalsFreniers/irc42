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
        void sendTo(std::string msg, int clientIndex);
        void broadcast(std::string msg);
        void broadcastExcept(std::string msg, int exceptIndex);
        void stop();
        operator bool() const;
private:
        bool _exit;
        int _sock;
        std::vector<Client *> _clients;
        std::string _pass;
};
