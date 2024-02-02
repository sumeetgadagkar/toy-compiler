cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(toy-compiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(toy-compiler main.cpp)

add_subdirectory(lexer)

target_link_libraries(toy-compiler PRIVATE lexer)
