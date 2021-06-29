cmake_minimum_required(VERSION 3.19)
project(lamp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}")

add_executable(lamp main.cpp include/Lexer.h src/Lexer.cpp tests/lexer_test.cpp include/Utility.h)

target_link_libraries(lamp gtest)

enable_testing()
add_executable(
        lexer_test
        tests/lexer_test.cpp
        src/Lexer.cpp
)
target_link_libraries(
        lexer_test
        gtest
        gtest_main
        pthread
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
