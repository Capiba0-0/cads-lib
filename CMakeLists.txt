cmake_minimum_required(VERSION 3.21)
project(cads-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Politics
cmake_policy(SET CMP0135 NEW)

# Library (CADS)
set(LIB_NAME cads)

add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)