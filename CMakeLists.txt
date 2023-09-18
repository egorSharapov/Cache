cmake_minimum_required(VERSION 3.22)

project(cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "building tests for cache" OFF)
file(GLOB SRCS
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
)
add_executable(cache ${SRCS})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)