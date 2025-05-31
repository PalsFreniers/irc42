#pragma once

#include <fstream>
#include <string>
#include <cstdarg>

enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_FATAL,
};

const char *getLogStr(LogLevel l);

class Logger
{
public:
	Logger();
	Logger(const std::string &name);
	Logger(const Logger &src);
	Logger&operator=(const Logger &rhs);
	~Logger();

        void log(LogLevel lvl, const std::string fmt, ...);
        void logFormat(const std::string str, size_t &i, std::va_list lst);

        void setLogFile(const std::string &filename);
private:
	std::string _name;
        std::ostream &_logFile;
        std::ofstream _file;
};

extern Logger _log;
