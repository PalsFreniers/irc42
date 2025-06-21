#include "Server.hpp"
#include "Logger.hpp"
#include <csignal>


bool _exitServ = false;

//set signal handler for graceful shutdown on sig int and sig term
void signalHandler(int signum) {
        _log.log(LOG_INFO, "Received signal /d, shutting down server", signum);
        _log.log(LOG_INFO, "Exiting...");
        _exitServ = true;
}

int main(int c, char **v) {
        if (c < 3) {
                _log.log(LOG_FATAL, "Usage: /s <port> <password>", v[0]);
                return 1;
        }

        _log.setLogFile("server.log");

        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        char *endptr;
        long port = strtol(v[1], &endptr, 10);
        if (*endptr != '\0' || port < 1 || port > 65535) {
                _log.log(LOG_FATAL, "Invalid port number: /s", v[1]);
                return 1;
        }
        std::string pass = v[2];
        if(pass.empty()) {
                _log.log(LOG_FATAL, "Password cannot be empty");
                return 1;
        }
        
        try {
                Server server(port, pass);
                while (server) {
                        server.poll();
                        if (_exitServ) server.stop();
                }
        } catch (const std::exception &e) {
                _log.log(LOG_FATAL, "Server error: /s", e.what());
                return 1;
        }
        _log.log(LOG_INFO, "Server stopped successfully");
        return 0;
}
