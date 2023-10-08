cmake_minimum_required(VERSION 3.22)

project(cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "building tests for cache" ON)

file(GLOB LFU_SRCS
        "${PROJECT_SOURCE_DIR}/src/lfu.cpp"
)

file(GLOB IDEAL_SRCS
        "${PROJECT_SOURCE_DIR}/src/ideal.cpp"
)

add_executable(ideal ${IDEAL_SRCS})
add_executable(lfu ${LFU_SRCS})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)