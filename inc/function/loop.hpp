/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:08:04 by augougea          #+#    #+#             */
/*   Updated: 2024/08/20 20:08:07 by augougea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOOP_HPP
#define LOOP_HPP

void	pollDeclaration(std::vector<struct pollfd>& fds);
bool    addNewClient(std::vector<struct pollfd>& fds, Client &client);
void    newConnection(std::vector<struct pollfd>& fds, Server &server);
void	eraseClient(int index, Server &server);
void    messageClients(std::vector<struct pollfd>& fds, Server &server);

#endif
