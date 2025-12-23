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

# Link to your executable
target_link_libraries(your_target PRIVATE xale-logger pthread)
```

## Configuration

```cpp
// Enable/disable debug level
XaleLogger::Logger<void>::setIsDebugEnable(true);

// Enable/disable console output
XaleLogger::Logger<void>::setLogToConsole(true);

// Enable/disable file output (not yet implemented)
XaleLogger::Logger<void>::setLogToFile(false);
```

## Build the example

```bash
mkdir build && cd build
cmake ..
make
./xale-logger-debug
```

## Requirements

- pthread support

## License

See LICENSE file for details.

## Author

[axdelafuen](https://github.com/axdelafuen)
