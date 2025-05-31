#include "Server.hpp"
#include "Logger.hpp"
#include "utils.hpp"
#include <sys/poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>


Server::Server(int port, std::string pass) : _pass(pass), _exit(false), _sock(-1), _clients() {
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
        _log.log(LOG_INFO, "Server started on port /d", port);
}

Server::~Server() {
        _log.log(LOG_INFO, "Shutting down server");
        close(_sock);
        for (size_t i = 0; i < _clients.size(); ++i) {
                delete _clients[i];
        }
}

void Server::poll() {
        struct pollfd fds[_clients.size() + 1];
        fds[0].fd = _sock;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        for (size_t i = 0; i < _clients.size(); ++i) {
                fds[i + 1].fd = _clients[i]->getSocket();
                fds[i + 1].events = POLLIN | POLLOUT;
                fds[i + 1].revents = 0;
        }
        int ret = ::poll(fds, _clients.size() + 1, -1);
        if (ret < 0) throw std::runtime_error("Failed to poll sockets");
        for (size_t i = 0; i < sizeof(fds) / sizeof(struct pollfd); ++i) {
                if (i != 0 && (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))) {
                        _log.log(LOG_INFO, "Client disconnected /d", fds[i].fd);
                        delete _clients[i - 1];
                        _clients.erase(_clients.begin() + i - 1);
                }
                if (fds[i].revents & POLLIN) {
                        if (i == 0) {
                                _clients.push_back(new Client(*this));
                                _log.log(LOG_INFO, "New client connected /d", _clients.back()->getSocket());
                        } else {
                                std::string msg = _clients[i - 1]->recv();
                                if(msg.empty()) {
                                        _log.log(LOG_INFO, "Client disconnected /d", _clients[i - 1]->getSocket());
                                        delete _clients[i - 1];
                                        _clients.erase(_clients.begin() + i - 1);
                                        continue;
                                }
                                int toTrim = 0;
                                if(msg[msg.length() - 1] == '\n' || msg[msg.length() - 1] == '\r') toTrim++;
                                if(msg.length() > 1 && (msg[msg.length() - 2] == '\n' || msg[msg.length() - 2] == '\r')) toTrim++;
                                msg = msg.substr(0, msg.length() - toTrim);
                                parseAndExec(msg, i - 1);
                        }
                }
        }
}

// parse irc commands and execute them
// only modify this the rest of the server works perfectly
void Server::parseAndExec(std::string msg, int currentClientIndex) {
        if (msg.empty()) return;
        _log.log(LOG_DEBUG, "Received message from /d: /s", currentClientIndex, msg.c_str());
        if (currentClientIndex < 0 || currentClientIndex >= static_cast<int>(_clients.size())) {
                _log.log(LOG_DEBUG, "Client index out of range: /d", currentClientIndex);
                throw std::out_of_range("Client index out of range");
        }
        Client &currentClient = *_clients[currentClientIndex];
        if (msg[0] == '/') {
                std::string command = msg.substr(1);
                if(command.substr(0, 5) == "pass ") {
                        if (currentClient.getAuth()) {
                                currentClient.send("You are already authenticated");
                        } else if (command.length() > 5 && command.substr(5) == _pass) {
                                currentClient.setAuth(true);
                                currentClient.send("Authentication successful");
                        } else {
                                currentClient.send("Authentication failed");
                        }
                } else if(command.substr(0, 4) == "say ") {
                        if(currentClient.getAuth()) {
                                std::string msgToSend = currentClient.nick() + " says: " + command.substr(4);
                                broadcastExcept(msgToSend, currentClientIndex);
                        } else {
                                currentClient.send("You must authenticate first");
                        }
                } else if(command.substr(0, 5) == "nick ") {
                        std::string newNick = command.substr(5);
                        if (newNick.empty()) {
                                currentClient.send("Nickname cannot be empty");
                        } else {
                                currentClient.setNick(newNick);
                                currentClient.send("Nickname changed to " + newNick);
                        }
                } else if(command == "stop") {
                        _exit = true;
                } else {
                        currentClient.send("Unknown command: " + command);
                }
        } else {
                if (!currentClient.getAuth()) {
                        currentClient.send("You must authenticate first");
                        return;
                }
                broadcast(msg);
        }
}

void Server::broadcast(std::string msg) {
        for (size_t i = 0; i < _clients.size(); ++i) {
                _clients[i]->send(msg);
        }
}

void Server::broadcastExcept(std::string msg, int exceptIndex) {
        for (size_t i = 0; i < _clients.size(); ++i) {
                if (static_cast<int>(i) != exceptIndex) {
                        _clients[i]->send(msg);
               }
        }
}

int Server::accept() {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = ::accept(_sock, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) throw std::runtime_error("Failed to accept connection");
        setnonblocking(newsockfd);
        return newsockfd;
}

Server::operator bool() const {
        return !_exit;
}

void Server::stop() {
        _exit = true;
}
