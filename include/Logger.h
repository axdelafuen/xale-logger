#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>
#include <iomanip>
#if defined(__linux__) || defined(linux) || defined(__GNUG__)
    #include <cxxabi.h>
#endif

#include "LoggerConfig.h"

constexpr auto STRING_COLOR_BLACK = "\033[0m";
constexpr auto STRING_COLOR_CYAN = "\033[36m";
constexpr auto STRING_COLOR_ORANGE = "\033[33m";
constexpr auto STRING_COLOR_RED = "\033[31m";

namespace Xale::Logger
{
    /**
	 * Xale Logger instance class
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
            static void setLogFilePath(const std::string& filePath);
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
            bool isLogFilePathSet();
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
     * Retrieve the singleton instance of Logger
     * 
	 * @return Logger<T>& The singleton instance
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
     * 
	 * @param enable true to enable debug level, false to disable
     */
    template<typename T>
    void Logger<T>::setIsDebugEnable(bool enable)
    {
        LoggerConfig::isDebugEnable = enable;
    }
    
    /**
     * Enable or disable the console logging behavior
     * 
	 * @param enable true to enable console logging, false to disable
     */
    template<typename T>
    void Logger<T>::setLogToConsole(bool enable)
    {
        LoggerConfig::logToConsole = enable;
    }

    /**
     * Enable or disable the file logging behavior
     * 
	 * @param enable true to enable file logging, false to disable
     */
    template<typename T>
    void Logger<T>::setLogToFile(bool enable)
    {
        LoggerConfig::logToFile = enable;
    }

    /**
     * Set the log file path
     * 
	 * If the file path is empty, the log file will be closed
     * 
	 * Else the log file will be opened in append mode
     * 
	 * @param filePath The log file path
     */
	template<typename T>
    void Logger<T>::setLogFilePath(const std::string& filePath)
    {
		if (filePath.empty())
			LoggerConfig::logFile.close();
        else
        {
			LoggerConfig::logFile.open(filePath, std::ios::app);
        }
    }

    /**
     * Log a message
     * 
	 * @param level The log level
	 * @param message The message to log
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
            if (level == LogLevel::ERROR)
            {
                std::cerr << getLevelColor(level) << logMessage
                    << STRING_COLOR_BLACK << std::endl;
            }
            else {
                std::cout << getLevelColor(level) << logMessage
                    << STRING_COLOR_BLACK << std::endl;
            }
        }

        if (LoggerConfig::logToFile)
        {
            if (isLogFilePathSet())
				LoggerConfig::logFile << logMessage << std::endl;
            else
            {
                std::cerr 
                    << STRING_COLOR_RED
                    << "[Logger Error] Log file path is not set."
                    << STRING_COLOR_BLACK << std::endl;
            }
        }
    }

    /**
	 * Log a message as debug
     * 
	 * @see log
     * @param message The message to log
     */
    template<typename T>
    void Logger<T>::debug(const std::string& message)
    {
        log(LogLevel::DEBUG, message);
    }
    
    /**
	 * Log a message as information
     * 
	 * @see log
     * @param message The message to log
     */
    template<typename T>
    void Logger<T>::info(const std::string& message)
    {
        log(LogLevel::INFORMATION, message);
    }
    
    /**
	 * Log a message as warning
     * 
	 * @see log
     * @param message The message to log
     */
    template<typename T>
    void Logger<T>::warning(const std::string& message)
    {
        log(LogLevel::WARNING, message);
    }
    
    /**
	 * Log a message as error
     * 
	 * @see log
     * @param message The message to log
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
	 * Get the current timestamp as a formatted string
     * 
	 * @return std::string The current timestamp in "YYYY-MM-DD HH:MM:SS.mmm" format
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
	 * Get the string representation of a log level
     * 
	 * @return std::string The string representation of the log level
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
	 * Get the color code for a log level
     * 
	 * @return std::string The color code for the log level
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
	 * Get the class name of the template type T
     * 
	 * @return std::string The class name of T
     */
    template<typename T>
    std::string Logger<T>::getClassName()
    {
        const char* mangled = typeid(T).name();

#if defined(__linux__) || defined(linux) || defined(__GNUG__)
        int status = 0;
        char* fullNameAbi = abi::__cxa_demangle(mangled, 0, 0, &status);
        
        if (status == 0)
            return fullNameAbi;
        
        std::string fullName(mangled);
        size_t pos = fullName.find_last_of(" :");
        
        if (pos != std::string::npos)
            return fullName.substr(pos + 1);

        return fullName;

#elif defined(_WIN32)
        std::string fullName(mangled);
        size_t pos = fullName.find_last_of(" :");

        if (pos != std::string::npos)
            return fullName.substr(pos + 1);

#else
		std::string fullName(mangled);

#endif
        return fullName;
    }


    /**
	* Check if the log file path is set
    * 
	* @return true if the log file path is set, false otherwise
    */
    template<typename T>
    bool Logger<T>::isLogFilePathSet()
    {
        return LoggerConfig::logFile.is_open();
    }
}

#endif // LOGGER_H
