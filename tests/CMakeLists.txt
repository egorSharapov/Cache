cmake_minimum_required(VERSION 3.22)
project(cache_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  cache_test
  cache_test.cpp
)

target_include_directories(cache_test PRIVATE ../src)

target_link_libraries(
  cache_test
  gtest
)



include(GoogleTest)
gtest_discover_tests(cache_test)
