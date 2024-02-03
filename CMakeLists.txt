cmake_minimum_required(VERSION 3.10)

project(LongArithmetic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LongNumber lib/long_number.cpp)

add_executable(LongArithmetic main.cpp)

target_link_libraries(LongArithmetic LongNumber)
