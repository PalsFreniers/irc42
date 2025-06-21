#pragma once

#include "Client.hpp"
#include <vector>

class AServer {
public:
        AServer(int port, std::string pass);
        virtual ~AServer();
        void poll();
        int accept();
        void sendTo(std::string msg, int clientIndex);
        void broadcast(std::string msg);
        void broadcastExcept(std::string msg, int exceptIndex);
        void stop();
        operator bool() const;
protected:
        bool _checkPass(std::string pass);
        Client *_getClient(int idx);
        virtual void _parseAndExec(std::string msg, int currentClientIndex) = 0;
        virtual void _acceptClient(int clientIndex) = 0;
        virtual void _disconnectClient(int clienIndex) = 0;
private:
        bool _exit;
        int _sock;
        std::vector<Client *> _clients;
        std::string _pass;
};
