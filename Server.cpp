#include "Server.hpp"
#include "Logger.hpp"

Server::Server(int port, std::string pass): AServer(port, pass) {}

Server::~Server() {}

void Server::_acceptClient(int clientIndex) {}
void Server::_disconnectClient(int clientIndex) {}

// parse irc commands and execute them
// only modify this the rest of the server works perfectly
void Server::_parseAndExec(std::string msg, int currentClientIndex) {
        if (msg.empty()) return;
        _log.log(LOG_DEBUG, "Received message from /d: /s", currentClientIndex, msg.c_str());
        Client &currentClient = *_getClient(currentClientIndex);
        if (msg[0] == '/') {
                std::string command = msg.substr(1);
                if(command.substr(0, 5) == "pass ") {
                        if (currentClient.getAuth()) {
                                currentClient.send("You are already authenticated");
                        } else if (command.length() > 5 && _checkPass(command.substr(5))) {
                                currentClient.setAuth();
                                currentClient.send("Authentication successful");
                        } else {
                                currentClient.send("Authentication failed");
                        }
                } else if(command.substr(0, 4) == "say ") {
                        if(currentClient.getAuth()) {
                                std::string msgToSend = "client says: " + command.substr(4);
                                broadcastExcept(msgToSend, currentClientIndex);
                        } else {
                                currentClient.send("You must authenticate first");
                        }
                } else if(command.substr(0, 5) == "nick ") {
                        std::string newNick = command.substr(5);
                        if (newNick.empty()) {
                                currentClient.send("Nickname cannot be empty");
                        } else {
                                currentClient.send("Nickname changed to " + newNick);
                        }
                } else if(command == "stop") {
                        stop();
                } else {
                        currentClient.send("Unknown command: " + command);
                }
        } else {
                if (!currentClient.getAuth()) {
                        currentClient.send("You must authenticate first");
                        return;
                }
                broadcast(msg);
        }
}
