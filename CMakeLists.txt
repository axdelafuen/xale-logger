cmake_minimum_required(VERSION 3.10)
project(XaleLoggerDebug VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${EXAMPLES_DIR}/*.cpp")

add_executable(xale-logger-debug ${SOURCES})

target_include_directories(xale-logger-debug PRIVATE ${INCLUDE_DIR})

if(UNIX)
    target_compile_options(xale-logger-debug PRIVATE -Wall -Wextra -pthread)
endif()
