#include "Server.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>

//std cpp98

Server::Server(int port, std::string pass) : _pass(pass) {
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
}

Server::~Server() {
        close(_sock);
        for (size_t i = 0; i < _clients.size(); ++i) {
                _clients[i].~Client();
        }
}

// Poll for all sockets events using the poll() system call
// Note that we need to check for input/output events on all sockets
// and handle them accordingly
// on POLLHUP we need to close the socket and remove it from the list after acquiring the rest of the current buffer

void Server::poll() {
        struct pollfd fds[_clients.size() + 1];
        fds[0].fd = _sock;
        fds[0].events = POLLIN;
        for (size_t i = 0; i < _clients.size(); ++i) {
                fds[i + 1].fd = _clients[i].getSocket();
                fds[i + 1].events = POLLIN | POLLOUT;
        }
        int ret = ::poll(fds, _clients.size() + 1, -1);
        if (ret < 0) throw std::runtime_error("Failed to poll sockets");
        for (size_t i = 0; i < _clients.size() + 1; ++i) {
                if (fds[i].revents & POLLIN) {
                        if (i == 0) {
                                _clients.push_back(Client(*this));
                        } else {
                                parseAndExec(_clients[i - 1].recv(), i - 1);
                        }
                }
                if (fds[i].revents & POLLHUP) {
                        close(fds[i].fd);
                        _clients.erase(_clients.begin() + i - 1);
                }
        }
}

void Server::parseAndExec(std::string msg, int currentClientIndex) {
        if (msg.empty()) return;
        _clients[currentClientIndex].send(msg);
}

int Server::accept() {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = ::accept(_sock, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) throw std::runtime_error("Failed to accept connection");
        setnonblocking(newsockfd);
        return newsockfd;
}
