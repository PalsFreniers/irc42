#include "Server.hpp"
#include "sha256.hpp"
#include <cstring>
#include <ios>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <exception>
#include "utils.hpp"
#include <cerrno>
#include <sstream>

#define foreach(x, v) for(__typeof__((v).begin()) x = (v).begin(); x != (v).end(); x++)
#define POLL_TIMEOUT 1000

Server::Server() : _port(8080), _sockfd(-1) {
        sha256("admin", _pass);
}

Server::~Server() {
        foreach(fd, _pfds) {
                if(fd->fd >= 0) close(fd->fd);
        }
        if(_sockfd >= 0) close(_sockfd);
}

Server::Server(int port, std::string pass) : _port(port), _sockfd(-1) {
        sha256(pass, _pass); 
}

Server::Server(const Server &other) {
        *this = other;
}

Server &Server::operator=(const Server &other) {
        for(int i = 0; i < 32; i++) _pass[i] = other._pass[i];
        _port = other._port;
        return *this;
}

void Server::launch() {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(_sockfd < 0) {
                std::stringstream ss;
                ss << "unable to create socket";
                throw std::runtime_error(ss.str().c_str());
        }
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(_port);
        if(bind(_sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0) {
                std::stringstream ss;
                ss << "unable to bind to port " << _port;
                throw std::runtime_error(ss.str().c_str());
        }
        _addr = address;
        _addrLen = sizeof(_addr);
        if(listen(_sockfd, 100) < 0) {
                std::stringstream ss;
                ss << "unable to listen at port " << _port;
                throw std::runtime_error(ss.str().c_str());
        }
        struct pollfd pfd = {_sockfd, POLLIN, 0};
        _pfds.push_back(pfd);
        std::stringstream ss;
        ss << "Server started on port " << _port << " with password hash 0x" << std::hex <<
                _pass[0] <<
                _pass[1] <<
                _pass[2] <<
                _pass[3] <<
                _pass[4] <<
                _pass[5] <<
                _pass[6] <<
                _pass[7];
        LOG(LOG_INFO, ss.str());
}

void Server::accept() {
        int clfd = -1;
        if((clfd = ::accept(_sockfd, (struct sockaddr *)&_addr, &_addrLen)) < 0) {
                if(errno != EWOULDBLOCK && errno != EAGAIN) {
                        std::stringstream ss;
                        ss << "unable to accept client (" << strerror(errno) << ")";
                        throw std::runtime_error(ss.str().c_str());
                }
        } else {
                struct pollfd pfd = {clfd, POLLIN, 0};
                _pfds.push_back(pfd);
        }
}

void Server::disconnectClient(int fd) {

}

void Server::onClientMessage(int fd) {

}

void Server::run() {
        try {
                launch();
        } catch(std::exception e) {
                LOG(LOG_ERROR, e.what());
                return;
        }
        while(true) {
                try {
                        if(poll(_pfds.begin().base(), _pfds.size(), -1) < 0) throw 0;
                        foreach(pfd, _pfds) {
                                if(pfd->revents == 0) continue;
                                if(pfd->revents & POLLHUP) disconnectClient(pfd->fd);
                                if(pfd->revents & POLLIN) {
                                        if(pfd->fd == _sockfd) {
                                                accept();
                                                break;
                                        }
                                        onClientMessage(pfd->fd);
                                }
                        }
                } catch(std::exception e) {
                        LOG(LOG_ERROR, e.what());
                        return;
                }
        }
}

bool Server::isCorrectPass(std::string pass) {
        u32 shaPass[8];
        sha256(pass, shaPass);
        for(int i = 0; i < 8; i++) if(shaPass[i] != _pass[i]) return false;
        return true;
}
