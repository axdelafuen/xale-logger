#include "LoggerConfig.h"

namespace XaleLogger
{
    // -----------------------------------------------------
    // LoggerConfig members definitions
    // -----------------------------------------------------

    std::mutex LoggerConfig::mutex_;
    bool LoggerConfig::isDebugEnable = true;
    std::ofstream LoggerConfig::logFile;
    bool LoggerConfig::logToFile = false;
    bool LoggerConfig::logToConsole = true;
}

