#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>
#include <iomanip>
#include <cxxabi.h>

#include "LoggerConfig.h"

#define STRING_COLOR_BLACK "\033[0m"
#define STRING_COLOR_CYAN "\033[36m"
#define STRING_COLOR_ORANGE "\033[33m"
#define STRING_COLOR_RED "\033[31m"

namespace XaleLogger
{
    /**
     * Logger 
     */
    template<typename T>
    class Logger
    {
        public:
            ~Logger() = default;
            static Logger<T>& getInstance();
            static void setIsDebugEnable(bool enable);
            static void setLogToConsole(bool enable);
            static void setLogToFile(bool enable);
            void log(LogLevel level, const std::string& message);
            void debug(const std::string& message);
            void info(const std::string& message);
            void warning(const std::string& message);
            void error(const std::string& message);
        private:
            static std::unique_ptr<Logger<T>> instance;
            Logger() = default;
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;
            std::string getCurrentTimestamp();
            std::string levelToString(LogLevel level);
            std::string getLevelColor(LogLevel level);
            std::string getClassName();
    };


    // -----------------------------------------------------
    // Logger members definitions
    // -----------------------------------------------------
    
    template<typename T>
    std::unique_ptr<Logger<T>> Logger<T>::instance = nullptr;

    // -----------------------------------------------------
    // Logger public methods definitions
    // -----------------------------------------------------

    /**
     * TODO: write comments
     */
    template<typename T>
    Logger<T>& Logger<T>::getInstance()
    {
        std::lock_guard<std::mutex> lock(LoggerConfig::mutex_);
        if (!instance) {
            instance.reset(new Logger<T>());
        }
        return *instance;
    }

    /**
     * Enable or disable the debug level
     */
    template<typename T>
    void Logger<T>::setIsDebugEnable(bool enable)
    {
        LoggerConfig::isDebugEnable = enable;
    }
    
    /**
     * Enable or disable the console logging behavior
     */
    template<typename T>
    void Logger<T>::setLogToConsole(bool enable)
    {
        LoggerConfig::logToConsole = enable;
    }

    /**
     * Enable or disable the file logging behavior
     */
    template<typename T>
    void Logger<T>::setLogToFile(bool enable)
    {
        LoggerConfig::logToFile = enable;
    }

    /**
     * Log a message
     */
    template<typename T>
    void Logger<T>::log(LogLevel level, const std::string& message)
    {
        if (!LoggerConfig::isDebugEnable && level == LogLevel::DEBUG)
            return;

        std::lock_guard<std::mutex> lock(LoggerConfig::mutex_);

        std::stringstream ss;
        ss << "[" << getCurrentTimestamp() << "] "
           << "[" << levelToString(level) << "] "
           << "[" << getClassName() << "] "
           << message;

        std::string logMessage = ss.str();

        if (LoggerConfig::logToConsole)
        {
            std::cout << getLevelColor(level) << logMessage
                     << STRING_COLOR_BLACK << std::endl;
        }

        if (LoggerConfig::logToFile)
        {
            // TODO: Write `logMessage` to `logFile`
        }
    }

    /**
     * @see Log a message as debug
     */
    template<typename T>
    void Logger<T>::debug(const std::string& message)
    {
        log(LogLevel::DEBUG, message);
    }
    
    /**
     * @see Log a message as information
     */
    template<typename T>
    void Logger<T>::info(const std::string& message)
    {
        log(LogLevel::INFORMATION, message);
    }
    
    /**
     * @see Log a message as warning
     */
    template<typename T>
    void Logger<T>::warning(const std::string& message)
    {
        log(LogLevel::WARNING, message);
    }
    
    /**
     * @see Log a message as error
     */
    template<typename T>
    void Logger<T>::error(const std::string& message)
    {
        log(LogLevel::ERROR, message);
    }

    // -----------------------------------------------------
    // private methods definitions
    // -----------------------------------------------------
    
    /**
     * TODO: write comments
     */
    template<typename T>
    std::string Logger<T>::getCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    /**
     * TODO: write comments
     */
    template<typename T>
    std::string Logger<T>::levelToString(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::DEBUG:           return "DEBUG";
            case LogLevel::INFORMATION:     return "INFORMATION";
            case LogLevel::WARNING:         return "WARNING";
            case LogLevel::ERROR:           return "ERROR";
            default:                        return "UNKNOWN";
        }
    }

    /**
     * TODO: write comments
     */
    template<typename T>
    std::string Logger<T>::getLevelColor(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::DEBUG:           return STRING_COLOR_CYAN;
            case LogLevel::INFORMATION:     return STRING_COLOR_BLACK;
            case LogLevel::WARNING:         return STRING_COLOR_ORANGE;
            case LogLevel::ERROR:           return STRING_COLOR_RED;
            default:                        return STRING_COLOR_BLACK;
        }
    }

    /**
     * TODO: write comments
     */
    template<typename T>
    std::string Logger<T>::getClassName()
    {
        const char* mangled = typeid(T).name();
        int status = 0;
        char* fullNameAbi = abi::__cxa_demangle(mangled, 0, 0, &status);
        
        if (status == 0)
            return fullNameAbi;
        
        std::string fullName(mangled);
        size_t pos = fullName.find_last_of(" :");
        
        if (pos != std::string::npos)
            return fullName.substr(pos + 1);

        return fullName;
    }
}

#endif // LOGGER_H

