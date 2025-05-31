#pragma once

#include <string>

class Server;

class Client {
public:
        Client(Server &serv);
        ~Client();
        void send(std::string data);
        std::string recv();
        std::string getBuffer() const;
        int getSocket() const;
        bool getAuth() const;
        void setAuth(bool auth);
        const std::string &nick() const;
        void setNick(const std::string &nick);
private:
        bool _auth;
        int _sock;
        std::string _buf;
        std::string _nick;
};
