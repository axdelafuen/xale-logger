#ifndef XALE_LOGGER_TESTS_HELPER_CPP
#define XALE_LOGGER_TESTS_HELPER_CPP

#include <iostream>
#include <sstream>
#include <string>

namespace Xale::Logger::Tests
{
    /**
     * Helper to capture std::cout output for a scoped block.
     */
    class CoutCapture
    {
        public:
            CoutCapture() : oldBuf_(std::cout.rdbuf(stream_.rdbuf())) 
            {}

            ~CoutCapture()
            {
                std::cout.rdbuf(oldBuf_);
            }

            std::string str() const
            {
                return stream_.str();
            }

        private:
            std::ostringstream stream_;
            std::streambuf* oldBuf_;
    };
}

#endif // XALE_LOGGER_TESTS_HELPER_CPP
