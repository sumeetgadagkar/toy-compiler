cmake_minimum_required(VERSION 3.10)

project(toy-compiler)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(toy-compiler main.cpp)

set_property(TARGET toy-compiler PROPERTY CXX_STANDARD 17)

add_subdirectory(lexer)

target_link_libraries(toy-compiler PRIVATE lexer)
