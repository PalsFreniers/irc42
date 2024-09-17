/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:01:10 by augougea          #+#    #+#             */
/*   Updated: 2024/08/21 16:21:16 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/includes.hpp"

Server::Server(int port, int max_clients)
        : _port(port),
        _max_clients(max_clients) {

}

Server::Server(Server const &src) {
        *this = src;
}

Server::~Server() {

}

Server &Server::operator=(Server const &src) { 
        if(this != &src) {
                _port = src._port;
                _max_clients = src._max_clients;
                _socket = src._socket;
                _address = src._address;
        }
        return *this;
}

int Server::getSocket() {
        return _socket;
}

void Server::setSocket(int src) {
        _socket = src;
}

sockaddr_in &Server::getAddress() {
        return _address;
}

void Server::setAddress(sockaddr_in src) {
        _address = src;
}

std::vector<struct pollfd> &Server::getFds() {
        return _fds;
}

void Server::setFds(std::vector<struct pollfd> &src) {
        _fds = src;
}

std::vector<Client> &Server::getClients() {
        return _client;
}

void Server::initializeServer() {
        int opt = 1;
        _address.sin_family = AF_INET;
        _address.sin_port = htons(_port);
        _address.sin_addr.s_addr = INADDR_ANY;
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if(_socket < 0) goto error;
        if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) goto clerr;
        if(bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0) goto clerr;
        if(listen(_socket, _max_clients) < 0) goto clerr;
clerr:
        close(_socket);
error:
        throw std::runtime_error("Error");
}

void Server::initializePoll() {
    _fds.resize(_max_clients + 1);
    for(size_t i = 0; i < _fds.size(); ++i) _fds[i].fd = -1;
    _fds[0].fd = _socket;
    _fds[0].events = POLLIN;
}

void Server::addClient(Client &client) {
    _client.push_back(client);
}

void Server::closeFdandSocket() {
    close(_socket);
    for(size_t i = 1; i < _fds.size(); ++i) if(_fds[i].fd >= 0) close(_fds[i].fd);
}
