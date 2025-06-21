#include "Client.hpp"
#include "AServer.hpp"
#include <unistd.h>
#include <sys/socket.h>

Client::Client(AServer &serv) : _sock(serv.accept()), _auth(false), _buf("") {}

Client::~Client() {
        close(_sock);
}

void Client::send(std::string data) {
        data += "\r\n";
        ::send(_sock, data.c_str(), data.size(), 0);
}

std::string Client::recv() {
        char buf[1024];
        int ret = ::recv(_sock, buf, sizeof(buf), 0);
        if (ret <= 0) {
                return "";
        }
        _buf.append(buf, ret);
        std::string data = "";
        bool CRLF = false;
        size_t pos = _buf.find("\r\n");
        if(pos != std::string::npos) CRLF = true;
        if (pos == std::string::npos) {
                pos = _buf.find("\n");
                CRLF = false;
        }
        if (pos == std::string::npos) {
                pos = _buf.find("\r");
                CRLF = false;
        }
        if (pos != std::string::npos) {
                data = _buf.substr(0, pos + (CRLF ? 2 : 1));
                _buf.erase(0, pos + (CRLF ? 2 : 1));
        }
        return data;
}

int Client::_getSocket() const {
        return _sock;
}

bool Client::getAuth() const {
        return _auth;
}

void Client::setAuth() {
        _auth = true;
}

void Client::unsetAuth() {
        _auth = false;
}
