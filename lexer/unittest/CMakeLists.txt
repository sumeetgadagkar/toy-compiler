include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "t*.cpp")

add_executable(lexer_tests ${TEST_SOURCES})

target_link_libraries(lexer_tests
  gtest
  gtest_main
  lexer
)

include(GoogleTest)
gtest_discover_tests(lexer_tests 
