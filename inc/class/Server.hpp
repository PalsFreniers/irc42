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

#ifndef SERVER_HPP
#define SERVER_HPP

//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <stdexcept>
//#include <unistd.h>
//#include <vector>
//#include <poll.h>
//#include <iostream>
//#include <cstring>
//#include <cerrno>
//#include "Client.hpp"

#include "../includes.hpp"

class Client ;

class Server
{
	private :
	
	int				_port;
	int				_max_clients;
	int 				_socket;
	sockaddr_in			_address;
	std::vector<struct pollfd>	_fds;
	std::vector<Client>		_client;

	public :

/*************************************** Constructeur ***************************************/

	Server(int port, int max_clients);
	Server(Server const &src);

/*************************************** Destructeur ****************************************/

	~Server(void);

/***************************************** Operateur ****************************************/

	Server &operator=(Server const &src);

/***************************************** Get/Set ******************************************/

	int 						getSocket(void);
	void 						setSocket(int src);
	sockaddr_in 				&getAddress(void);
	void 						setAddress(sockaddr_in src);
	std::vector<struct pollfd>	&getFds(void);
	void 						setFds(std::vector<struct pollfd> &src);
	std::vector<Client>			&getClients(void);
	
/***************************************** Fonctions ****************************************/

	void 		initializeServer(void);
	void    	initializePoll(void);
	void    	closeFdandSocket(void);
	void		addClient(Client &client);

};

#endif
