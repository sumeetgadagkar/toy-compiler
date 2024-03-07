cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(toy-compiler)

enable_testing()

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_executable(toy-compiler main.cpp)

add_subdirectory(lexer)

target_link_libraries(toy-compiler PRIVATE lexer)
