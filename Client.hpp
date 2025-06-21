#pragma once

#include <string>

class AServer;

class Client {
public:
        Client(AServer &serv);
        virtual ~Client();
        void send(std::string data);
        std::string recv();
        bool getAuth() const;
        void setAuth();
        void unsetAuth();
        int _getSocket() const;
private:
        bool _auth;
        int _sock;
        std::string _buf;
};
