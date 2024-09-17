/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augougea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 17:12:22 by augougea          #+#    #+#             */
/*   Updated: 2024/08/21 16:25:26 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.hpp"

void signalHandler(int signum) 
{
    std::cout << "Signal reçu : " << signum << std::endl;
}

int main() {
        Server myServer(8080, 3);
        signal(SIGINT, signalHandler);
        try {
                myServer.initializeServer();
                myServer.initializePoll();
                while(true) {
                        pollDeclaration(myServer.getFds());
                        newConnection(myServer.getFds(), myServer);
                        messageClients(myServer.getFds(), myServer);
                }
        } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
        }
        myServer.closeFdandSocket();
        return 0;
}
