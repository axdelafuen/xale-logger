#include <string.h>

#include "Logger.h"

class TestingClass
{
    public:
        TestingClass();
        void run();
    private:
        XaleLogger::Logger<TestingClass>& logger;
};

TestingClass::TestingClass() : logger(XaleLogger::Logger<TestingClass>::getInstance())
{
    logger.debug("TestingClass created!");
}

void TestingClass::run() 
{
    logger.info("Testing class is running...");
}

int main(int argc, char *argv[])
{
    auto& logger = XaleLogger::Logger<void>::getInstance();

    if (argc > 1)
    {
        logger.error("No arguments expected !");
        return -1;
    }

    // Disable globally debug level
    //XaleLogger::Logger<void>::setIsDebugEnable(false);

    TestingClass testVar;
    testVar.run();

    logger.debug("Void debug in main!");

    return 0;
}

