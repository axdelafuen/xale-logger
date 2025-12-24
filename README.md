# xale-logger

A lightweight, c++ logging library with class identification and configurable output levels.

## Integration

### Method 1: Copy Source Files

Copy the following files into your project:

```
include/Logger.h
include/LoggerConfig.h
src/LoggerConfig.cpp
```

Add to your compilation:

```bash
g++ -std=c++17 -pthread your_code.cpp src/LoggerConfig.cpp -I include/ -o your_program
```

### Method 2: CMake Integration

Add `FetchContent` to your `CMakeLists.txt` then:

```cmake
include(FetchContent)
FetchContent_Declare(xale-logger
    GIT_REPOSITORY https://github.com/axdelafuen/xale-logger.git
    GIT_TAG main)
FetchContent_MakeAvailable(xale-logger)

target_link_libraries(your_target PRIVATE xale-logger)
```

## Build the example

```bash
mkdir build && cd build
cmake ..
make
./xale-logger-debug
```

## Usage

```cpp
auto& logger = Xale::Logger::Logger<T>::getInstance();
logger.info("Information message");
logger.debug("Debug message");
logger.error("Error message");
```

Output:

```
<Black-colored>[YYYY-MM-DD hh:mm:ss] [INFORMATION] [Namespace::T] Information message<End-color>
<Cyan-colored>[YYYY-MM-DD hh:mm:ss] [DEBUG] [Namespace::T] Debug message<End-color>
<Red-colored>[YYYY-MM-DD hh:mm:ss] [ERROR] [Namespace::T] Error message<End-color>
```

**Configuration:**

```cpp
// Enable/disable debug level
Xale::Logger::Logger<void>::setIsDebugEnable(true);

// Enable/disable console output
Xale::Logger::Logger<void>::setLogToConsole(true);

// Enable/disable file output
Xale::Logger::Logger<void>::setLogToFile(false);
Xale::Logger::Logger<void>::setLogFilePath("path/to/logfile.log");
```

## Requirements

- pthread support

## License

See LICENSE file for details.

## Author

[axdelafuen](https://github.com/axdelafuen)
