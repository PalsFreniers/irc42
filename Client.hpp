#pragma once

#include <string>

class Server;

class Client {
public:
        Client(Server serv);
        ~Client();
        void send(std::string data);
        std::string recv();
        std::string getBuffer() const;
        int getSocket() const;
private:
        int _sock;
        std::string _buf;
};
