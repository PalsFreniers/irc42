/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 17:12:22 by augougea          #+#    #+#             */
/*   Updated: 2024/08/17 17:13:30 by augougea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.hpp"

void signalHandler(int signum) 
{
    std::cout << "Signal reçu : " << signum << std::endl;
}

int main() {

    Server myServer(8080, 3);
    std::vector<struct pollfd>& fds = myServer.getFds();

    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    try
    {
        myServer.initializeServer();
        myServer.initializePoll();
        while (true) 
        {
            pollDeclaration(fds);
            newConnection(fds, myServer);
            messageClients(fds, myServer);
        }
    }
    catch(std::exception &e) { std::cerr << e.what() << std::endl; }

    myServer.closeFdandSocket();

    return 0;
}
