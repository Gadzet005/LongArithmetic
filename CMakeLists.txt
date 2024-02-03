cmake_minimum_required(VERSION 3.10)
project(LongArithmetic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2)

file(GLOB SOURCE_FILES src/*.cpp)
add_library(Source ${SOURCE_FILES})

add_executable(Calculator calculator.cpp)
add_executable(Pi pi.cpp)

target_link_libraries(Calculator Source)
target_link_libraries(Pi Source)
