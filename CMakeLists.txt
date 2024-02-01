cmake_minimum_required(VERSION 3.10)

project(LongArithmetic)

add_library(LongNumber long_number.cpp)

add_executable(LongArithmetic main.cpp)

target_link_libraries(LongArithmetic LongNumber)
