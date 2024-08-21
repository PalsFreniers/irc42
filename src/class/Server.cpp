/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:01:10 by augougea          #+#    #+#             */
/*   Updated: 2024/08/19 19:01:12 by augougea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/includes.hpp"

/*************************************** Constructeur ***************************************/

Server::Server(int port, int max_clients) : _port(port), _max_clients(max_clients) {}

Server::Server(Server const &src) { *this = src; }

/*************************************** Destructeur ****************************************/

Server::~Server(void) {}
	
/***************************************** Operateur ****************************************/

Server &Server::operator=(Server const &src) 
{ 
	if (this != &src)
	{
		this->_port = src._port;
		this->_max_clients = src._max_clients;
		this->_socket = src._socket;
		this->_address = src._address;
	}
	return (*this);
}

/***************************************** Get/Set ******************************************/

int Server::getSocket(void) { return (this->_socket); }
void Server::setSocket(int src) { this->_socket = src; }
sockaddr_in &Server::getAddress(void) { return (this->_address); }
void Server::setAddress(sockaddr_in src) { this->_address = src; }
std::vector<struct pollfd> &Server::getFds(void) { return (this->_fds); }
void Server::setFds(std::vector<struct pollfd> &src) { this->_fds = src; }
std::vector<Client> &Server::getClients(void) { return (this->_client); }

/***************************************** Fonctions ****************************************/

void Server::initializeServer(void)
{
    int opt = 1;

    this->_address.sin_family = AF_INET;
    this->_address.sin_port = htons(this->_port);
    this->_address.sin_addr.s_addr = INADDR_ANY;

    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket < 0) {
        throw std::runtime_error("Error"); }

   	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        close(this->_socket);
        throw std::runtime_error("Error");
    }

    if (bind(this->_socket, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0)
	{
        close(this->_socket);
        throw std::runtime_error("Error");
    }

    if (listen(this->_socket, this->_max_clients) < 0)
	{
        close(this->_socket);
        throw std::runtime_error("Error");
    }
}

void    Server::initializePoll(void)
{
    this->_fds.resize(_max_clients + 1);
    for (size_t i = 0; i < this->_fds.size(); ++i)
        this->_fds[i].fd = -1;
    this->_fds[0].fd = this->_socket;
    this->_fds[0].events = POLLIN;
}

void    Server::addClient(Client &client)
{
    this->_client.push_back(client);
}

void    Server::closeFdandSocket(void)
{
    close (this->_socket);
    for (size_t i = 1; i < this->_fds.size(); ++i) 
    {
        if (this->_fds[i].fd >= 0)
            close (this->_fds[i].fd);
    }  
}
