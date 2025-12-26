#include <string.h>

#include "Logger.h"

class TestingClass
{
    public:
        TestingClass();
        void run();
        void debug();
        void crash();
    private:
        Xale::Logger::Logger<TestingClass>& logger;
};

TestingClass::TestingClass() : logger(Xale::Logger::Logger<TestingClass>::getInstance())
{
    logger.debug("TestingClass created!");
}

void TestingClass::run() 
{
    logger.info("Testing class is running...");
}

void TestingClass::debug() 
{
    logger.debug("This is a debug message from TestingClass.");
}

void TestingClass::crash()
{
    logger.error("Unhandled crash !!");
}

int main(int argc, char *argv[])
{
    auto& logger = Xale::Logger::Logger<void>::getInstance();

    if (argc > 1)
    {
        logger.error("No arguments expected !");
        return -1;
    }

	// Basic class logging usage
    TestingClass testVar;
    testVar.run();

	// Basic void logging usage
    logger.debug("Void debug in main!");

	// Setup file logging
	Xale::Logger::Logger<void>::setLogToFile(true);
    Xale::Logger::Logger<void>::setLogFilePath("today.log");
	logger.info("This info should go to the log file.");

    // Disable globally debug level
    Xale::Logger::Logger<void>::setIsDebugEnable(false);
	logger.debug("This debug should NOT appear. (neither in the terminal nor in the file)");
    testVar.debug();

    // Test error on stderr
    testVar.crash();

    return 0;
}
