#include "Server.hpp"
#include <iostream>
#include <cstdlib>

int	main(int c, char *args[])
{
        char	*tmp;
        int		port;

        if (c != 3)
        {
                std::cerr << "usage : " << args[0] << " <port> <password>" << std::endl;
                return (1);
        }
        port = strtol(args[1], &tmp, 10);
        if (*tmp != 0)
                return (1);
        Server serv = Server(port, args[2]);
        serv.run();
        return (0);
}
