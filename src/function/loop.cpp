/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:02:55 by augougea          #+#    #+#             */
/*   Updated: 2024/08/20 20:02:58 by augougea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/includes.hpp"

void pollDeclaration(std::vector<struct pollfd>& fds)
{
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0) 
            throw std::runtime_error("Error");
}

bool    addNewClient(std::vector<struct pollfd>& fds, Client &client)
{
    for (size_t i = 1; i < fds.size(); ++i) 
    {
        if (fds[i].fd < 0) 
        {
            fds[i].fd = client.getSocket();
            fds[i].events = POLLIN;
            client.setIndex(i);
            std::cout << "Nouveau client connecté." << std::endl;
            return (true);
        }
    }
    std::cerr << "Erreur: nombre maximal de clients atteint." << std::endl;
    return (false);
}

void    newConnection(std::vector<struct pollfd>& fds, Server &server)
{
    if (fds[0].revents & POLLIN) 
    {
        Client new_client;
        new_client.initializeClient(server);
        if (!addNewClient(fds, new_client))
            close(new_client.getSocket());
        else
            server.addClient(new_client);
    }
}

void eraseClient(int index, Server &server)
{
    std::vector<Client> &clients = server.getClients();
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) 
    {
        if (it->getIndex() == index)
        {
            clients.erase(it);
            break ;
        }
    }
}

void    messageClients(std::vector<struct pollfd>& fds, Server &server)
{
    for (size_t i = 1; i < fds.size(); ++i) 
    {
        if (fds[i].fd < 0) 
            continue ;
        if (fds[i].revents & POLLIN) 
        {
            char buffer[BUFFER_SIZE] = {0};
            ssize_t bytesReceived = recv(fds[i].fd, buffer, sizeof(buffer), 0);
            if (bytesReceived < 1) 
            {
                std::cout << "Client déconnecté ou erreur." << std::endl;
                close(fds[i].fd);
                fds[i].fd = -1;
                eraseClient(i, server);
            }
            else
                std::cout << "Message from client: " << buffer << std::endl;
        }
    }
}
