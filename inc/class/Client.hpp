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

#ifndef CLIENT_HPP
#define CLIENT_HPP

//#include <unistd.h>
//#include <sys/socket.h> 
//#include <netinet/in.h>
//#include "Server.hpp"
#include "../includes.hpp"

class Server ;

class Client
{
	private :

	int 			_socket;
	sockaddr_in		_address;
	socklen_t 		_length;
	char			*_buffer;
	ssize_t 		_bytes_recv;
	int				_index;

	public :

/*************************************** Constructeur ***************************************/

	Client(void);
	Client(Client const &src);

/*************************************** Destructeur ****************************************/

	~Client(void);

/***************************************** Operateur ****************************************/

	Client &operator=(Client const &src);

/***************************************** Get/Set ******************************************/

	int			getSocket(void);
	void		setSocket(int src);
	char		*getBuffer(void);
	void		setBuffer(char *src);
	ssize_t 	getBytesRecv(void);
	void 		setBytesRecv(ssize_t src);
	int			getIndex(void);
	void		setIndex(int src);

/***************************************** Fonctions ****************************************/

	void		initializeClient(Server &server);

};

#endif
