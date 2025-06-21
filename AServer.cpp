#include "AServer.hpp"
#include "Logger.hpp"
#include "utils.hpp"
#include <string>
#include <sys/poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>


AServer::AServer(int port, std::string pass) : _pass(pass), _exit(false), _sock(-1), _clients() {
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0) throw std::runtime_error("Failed to create socket");
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
        if (bind(_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                close(_sock);
                throw std::runtime_error("Failed to bind socket");
        }
        if (listen(_sock, 5) < 0) {
                close(_sock);
                throw std::runtime_error("Failed to listen on socket");
        }
        setnonblocking(_sock);
        _log.log(LOG_INFO, "AServer started on port /d", port);
}

AServer::~AServer() {
        _log.log(LOG_INFO, "Shutting down server");
        close(_sock);
        for (size_t i = 0; i < _clients.size(); ++i) {
                delete _clients[i];
        }
}

void AServer::poll() {
        struct pollfd fds[_clients.size() + 1];
        fds[0].fd = _sock;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        for (size_t i = 0; i < _clients.size(); ++i) {
                fds[i + 1].fd = _clients[i]->_getSocket();
                fds[i + 1].events = POLLIN | POLLOUT;
                fds[i + 1].revents = 0;
        }
        int ret = ::poll(fds, _clients.size() + 1, -1);
        if (ret < 0) throw std::runtime_error("Failed to poll sockets");
        for (size_t i = 0; i < sizeof(fds) / sizeof(struct pollfd); ++i) {
                if (i != 0 && (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))) {
                        _disconnectClient(i - 1);
                        _log.log(LOG_INFO, "Client disconnected /d", fds[i].fd);
                        delete _clients[i - 1];
                        _clients.erase(_clients.begin() + i - 1);
                }
                if (fds[i].revents & POLLIN) {
                        if (i == 0) {
                                _clients.push_back(new Client(*this));
                                _acceptClient(_clients.size() - 1);
                                _log.log(LOG_INFO, "New client connected /d", _clients.size() - 1);
                        } else {
                                std::string msg = _clients[i - 1]->recv();
                                if(msg.empty()) {
                                        _disconnectClient(i - 1);
                                        _log.log(LOG_INFO, "Client disconnected /d", i - 1);
                                        delete _clients[i - 1];
                                        _clients.erase(_clients.begin() + i - 1);
                                        continue;
                                }
                                int toTrim = 0;
                                if(msg[msg.length() - 1] == '\n' || msg[msg.length() - 1] == '\r') toTrim++;
                                if(msg.length() > 1 && (msg[msg.length() - 2] == '\n' || msg[msg.length() - 2] == '\r')) toTrim++;
                                msg = msg.substr(0, msg.length() - toTrim);
                                _getClient(i - 1);
                                _parseAndExec(msg, i - 1);
                        }
                }
        }
}


void AServer::broadcast(std::string msg) {
        for (size_t i = 0; i < _clients.size(); ++i) {
                _clients[i]->send(msg);
        }
}

void AServer::broadcastExcept(std::string msg, int exceptIndex) {
        for (size_t i = 0; i < _clients.size(); ++i) {
                if (static_cast<int>(i) != exceptIndex) {
                        _clients[i]->send(msg);
               }
        }
}

int AServer::accept() {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = ::accept(_sock, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) throw std::runtime_error("Failed to accept connection");
        setnonblocking(newsockfd);
        return newsockfd;
}

AServer::operator bool() const {
        return !_exit;
}

void AServer::stop() {
        _exit = true;
}

bool AServer::_checkPass(std::string pass) {
        return pass == _pass;
}

Client *AServer::_getClient(int idx) {
        if (idx < 0 || idx >= static_cast<int>(_clients.size())) {
                _log.log(LOG_DEBUG, "Client index out of range: /d", idx);
                throw std::out_of_range("Client index out of range");
        }
        return _clients[idx];
}
