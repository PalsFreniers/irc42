#include "Client.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <sys/socket.h>

Client::Client(Server serv) : _sock(serv.accept()) {}

Client::~Client() {
        close(_sock);
}

void Client::send(std::string data) {
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
                data = _buf.substr(0, pos);
                if (CRLF) _buf.erase(0, pos + 2);
                else _buf.erase(0, pos + 1);
        }
        return data;
}

std::string Client::getBuffer() const {
        return _buf;
}

int Client::getSocket() const {
        return _sock;
}
