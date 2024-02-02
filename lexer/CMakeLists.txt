add_library(lexer Lexer.cpp)

target_include_directories(lexer PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(unittest)
