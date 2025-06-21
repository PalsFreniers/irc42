#pragma once

#include "AServer.hpp"
#include <string>

class Server : public AServer {
public:
        Server(int port, std::string name);
        virtual ~Server();
private:
        virtual void _parseAndExec(std::string msg, int clientIndex);
        virtual void _acceptClient(int clientIndex);
        virtual void _disconnectClient(int clientIndex);
};
