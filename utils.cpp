#include "utils.hpp"
#include <fcntl.h>
#include <iostream>
#include <stdexcept>

void setnonblocking(int sock) {
        int opt = fcntl(sock, F_GETFL);
        if (opt < 0) throw std::logic_error("unable to get socket flags");
        opt |= O_NONBLOCK;
        if (fcntl(sock, F_SETFL, opt) < 0) throw std::logic_error("unable to set socket flags");
}
