#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>

#define LOG(level, msg) { \
        std::cerr << level << " "; \
        std::time_t t = std::time(0); \
        std::tm *now = std::localtime(&t); \
        std::cerr << now->tm_year + 1900 << "-" \
                  << std::setfill('0') << std::setw(2) << now->tm_mon + 1 << "-" \
                  << std::setfill('0') << std::setw(2) << now->tm_mday << "#"; \
        std::cerr << std::setfill('0') << std::setw(2) << now->tm_hour << ":" \
                  << std::setfill('0') << std::setw(2) << now->tm_min << ":" \
                  << std::setfill('0') << std::setw(2) << (now->tm_sec + 1) % 60 << " "; \
        std::cerr << "=> " << msg << std::endl; \
}

#define LOG_DEBUG "[DEBUG]"
#define LOG_INFO "[INFO]"
#define LOG_WARNING "[WARNING]"
#define LOG_ERROR "[ERROR]"
