cmake_minimum_required(VERSION 3.10)
project(LongArithmetic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2)

file(GLOB SOURCE_FILES src/*.cpp)
add_library(Source ${SOURCE_FILES})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Calculator exe/calculator.cpp)
add_executable(Pi exe/pi.cpp)

target_link_libraries(Calculator Source)
target_link_libraries(Pi Source)

include_directories(include)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)