/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:01:10 by augougea          #+#    #+#             */
/*   Updated: 2024/08/19 19:01:12 by augougea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/includes.hpp"

/*************************************** Constructeur ***************************************/

Client::Client(void) {}

Client::Client(Client const &src) { *this = src; }

/*************************************** Destructeur ****************************************/

Client::~Client(void) {}
	
/***************************************** Operateur ****************************************/

Client &Client::operator=(Client const &src) 
{ 
	if (this != &src)
	{
		this->_socket = src._socket;
		this->_buffer = src._buffer;
		this->_bytes_recv = src._bytes_recv;
	}
	return (*this);
}

/***************************************** Get/Set ******************************************/

int Client::getSocket(void) { return (this->_socket); }
void Client::setSocket(int src) { this->_socket = src; }
char *Client::getBuffer(void) { return (this->_buffer); }
void Client::setBuffer(char *src) { this->_buffer = src; }
ssize_t Client::getBytesRecv(void) { return (this->_bytes_recv); }
void Client::setBytesRecv(ssize_t src) { this->_bytes_recv = src; }
int Client::getIndex(void) { return (this->_index); }
void Client::setIndex(int src) { this->_index = src; }

/***************************************** Fonctions ****************************************/

void	Client::initializeClient(Server &server)
{
	this->_length = sizeof(this->_address);
	this->_socket = accept(server.getSocket(), (struct sockaddr*)&this->_address, &this->_length);
    if (this->_socket < 0) 
        std::cerr << "Erreur lors de l'acceptation de la connexion: " << strerror(errno) << std::endl;
}
