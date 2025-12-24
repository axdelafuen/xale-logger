cmake_minimum_required(VERSION 3.10)
project(XaleLogger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(xale-logger STATIC
    src/LoggerConfig.cpp
)

target_include_directories(xale-logger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(UNIX)
    target_compile_options(xale-logger PRIVATE -Wall -Wextra)
    target_link_libraries(xale-logger PUBLIC pthread)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Debug executable
    add_executable(xale-logger-debug
        examples/BasicUsage.cpp
    )
    target_link_libraries(xale-logger-debug PRIVATE xale-logger)
    set_target_properties(xale-logger-debug PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    # Tests executable
    add_executable(xale-logger-tests
        tests/LoggerTests.cpp
        tests/TestsHelper.h)
    target_link_libraries(xale-logger-tests PRIVATE xale-logger)
    set_target_properties(xale-logger-tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
