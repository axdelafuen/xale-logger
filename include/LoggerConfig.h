#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

#include <string>
#include <mutex>
#include <thread>
#include <fstream>

namespace Xale::Logger
{
    /**
     * Log level enumeration type
     */ 
    enum LogLevel
    {
        DEBUG,
        INFORMATION,
        WARNING,
        ERROR,
    };

    /**
     * Logger configuration
     */
    class LoggerConfig
    {
        private:
            static std::mutex mutex_;
            static bool isDebugEnable;
            static bool logToConsole;
            static bool logToFile;
            static std::ofstream logFile;

            template<typename T>
            friend class Logger;
    };
}

#endif // LOGGER_CONFIG_H
