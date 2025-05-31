#include "Logger.hpp"
#include "colors.hpp"
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <bitset>

const char *getLogStr(LogLevel l) {
        switch(l) {
                case LOG_DEBUG: return BLU "[DEBUG]" CLR;
                case LOG_INFO: return GRN "[INFO]" CLR;
                case LOG_WARNING: return YLW "[WARNING]" CLR;
                case LOG_ERROR: return RED "[ERROR]" CLR;
                case LOG_FATAL: return RGB(127, 0, 0) "[FATAL]" CLR;
                default: return "[LOG]";
        }
}

Logger::Logger() : _name(""), _logFile(std::cerr) {}

Logger::Logger(const std::string &name) : _name(name), _logFile(std::cerr) {}

Logger::Logger(const Logger &src) : _name(src._name), _logFile(std::cerr) {}

Logger &Logger::operator=(const Logger &rhs) {
	this->_name = rhs._name;
	return *this;
}

Logger::~Logger() {
        if (_file.is_open()) {
                _file.close();
        }
}

void Logger::logFormat(const std::string str, size_t &i, std::va_list lst) {
        i++;
        switch(str[i]) {
                case '/': _logFile << '/'; break;
                case 's': _logFile << va_arg(lst, const char *); break;
                case 'c': _logFile << (char)va_arg(lst, int); break;
                case 'd': _logFile << va_arg(lst, int); break;
                case 'l': _logFile << va_arg(lst, long); break;
                case 'x': _logFile << "0x" << std::hex << va_arg(lst, int) << std::dec; break;
                case 'X': _logFile << "0x" << std::hex << va_arg(lst, long) << std::dec; break;
                case 'o': _logFile << "0" << std::oct << va_arg(lst, int) << std::dec; break;
                case 'b': _logFile << "0b" << std::bitset<sizeof(int) * 8>(va_arg(lst, int)) << std::dec; break;
                default: _logFile << "/" << str[i]; break;
        }
}

void Logger::log(LogLevel lvl, const std::string fmt, ...) {
#ifdef RELEASE
        if(lvl == LOG_DEBUG) return;
#endif // RELEASE
        std::va_list lst;
        _logFile << getLogStr(lvl) << (_name.empty() ? "" : " ") << _name << " => ";
        va_start(lst, fmt);
        for(size_t i = 0; i < fmt.length(); i++) {
                if(fmt[i] == '/' && i != fmt.length() - 1) logFormat(fmt, i, lst);
                else _logFile << fmt[i];
        }
        va_end(lst);
        _logFile << std::endl;
}

void Logger::setLogFile(const std::string &filename) {
        if (_file.is_open()) _file.close();
        _file.open(filename, std::ios::app);
        if (!_file.is_open()) throw std::runtime_error("Failed to open log file: " + filename);
        _logFile.rdbuf(_file.rdbuf());
        _logFile << "-------------------------------#-#-# Log file opened: " << filename << "#-#-#-------------------------------" << std::endl;
}
