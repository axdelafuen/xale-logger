#include "TestsHelper.h"
#include "Logger.h"

namespace Xale::Logger::Tests
{
    bool test_basic_debug_logging_to_console()
    {
        auto& logger = Logger<void>::getInstance();
        Logger<void>::setIsDebugEnable(true);
        Logger<void>::setLogToConsole(true);
        Logger<void>::setLogToFile(false);

        CoutCapture capture;
        logger.debug("Test debug message");

        const std::string output = capture.str();
        const bool hasLevel = output.find("[DEBUG]") != std::string::npos;
        const bool hasMessage = output.find("Test debug message") != std::string::npos;

        return hasLevel && hasMessage;
    }

    bool test_debug_disabled()
    {
        auto& logger = Logger<void>::getInstance();
        Logger<void>::setIsDebugEnable(false);
        Logger<void>::setLogToConsole(true);
        Logger<void>::setLogToFile(false);

        CoutCapture capture;
        logger.debug("This should not appear");

        const std::string output = capture.str();
        return output.find("This should not appear") == std::string::npos;
    }

    bool test_log_level_strings()
    {
        auto& logger = Logger<void>::getInstance();
        Logger<void>::setIsDebugEnable(true);
        Logger<void>::setLogToConsole(true);
        Logger<void>::setLogToFile(false);

        CoutCapture coutCapture;
        CerrCapture cerrCapture;
        logger.info("info");
        logger.warning("warn");
        logger.error("err");

        const std::string combinedOutput = coutCapture.str() + cerrCapture.str();
        const bool hasInfo = combinedOutput.find("[INFORMATION]") != std::string::npos;
        const bool hasWarn = combinedOutput.find("[WARNING]") != std::string::npos;
        const bool hasErr = combinedOutput.find("[ERROR]") != std::string::npos;

        return hasInfo && hasWarn && hasErr;
    }

    bool test_error_logging_to_stderr()
    {
        auto& logger = Logger<void>::getInstance();
        Logger<void>::setIsDebugEnable(true);
        Logger<void>::setLogToConsole(true);
        Logger<void>::setLogToFile(false);

        CoutCapture coutCapture;
        CerrCapture cerrCapture;
        logger.error("Error to stderr");

        const std::string coutOutput = coutCapture.str();
        const std::string cerrOutput = cerrCapture.str();

        const bool errorInCerr = cerrOutput.find("[ERROR]") != std::string::npos &&
                                 cerrOutput.find("Error to stderr") != std::string::npos;
        const bool errorNotInCout = coutOutput.find("Error to stderr") == std::string::npos;

        return errorInCerr && errorNotInCout;
    }

    bool test_file_logging_path_not_set()
    {
        auto& logger = Logger<void>::getInstance();
        Logger<void>::setIsDebugEnable(true);
        Logger<void>::setLogToConsole(false);
        Logger<void>::setLogToFile(true);
        Logger<void>::setLogFilePath("");

        logger.info("Should trigger file path error but not crash");
        return true;
    }
}

int main()
{
    using namespace Xale::Logger::Tests;

    int failed = 0;
    int passed = 0;
	int total = 5; // Should be determined automatically in a real test framework

    if (!test_basic_debug_logging_to_console())
    {
        std::cout << "[FAIL] test_basic_debug_logging_to_console" << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] test_basic_debug_logging_to_console" << std::endl;
		++passed;
    }

    if (!test_debug_disabled())
    {
        std::cout << "[FAIL] test_debug_disabled" << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] test_debug_disabled" << std::endl;
        ++passed;
    }

    if (!test_log_level_strings())
    {
        std::cout << "[FAIL] test_log_level_strings" << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] test_log_level_strings" << std::endl;
        ++passed;
    }

    if (!test_error_logging_to_stderr())
    {
        std::cout << "[FAIL] test_error_logging_to_stderr" << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] test_error_logging_to_stderr" << std::endl;
        ++passed;
    }

    if (!test_file_logging_path_not_set())
    {
        std::cout << "[FAIL] test_file_logging_path_not_set" << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] test_file_logging_path_not_set" << std::endl;
        ++passed;
    }

    if (failed != 0 || passed + failed != total)
    {
        std::cout << failed << " test(s) failed (" << passed << "/" << total << " passed)" << std::endl;
        return 1;
    }

    std::cout << std::endl << "All tests passeded (" << passed << "/" << total << " passed)" << std::endl;
    return 0;
}
