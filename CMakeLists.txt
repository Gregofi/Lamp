cmake_minimum_required(VERSION 3.19)
project(lamp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -I${PROJECT_SOURCE_DIR}")

add_executable(lamp main.cpp include/Lexer.h src/Lexer.cpp include/Utility.h include/Parser.h include/Nodes/Program.h include/Nodes/Node.h include/Nodes/Stmt/Stmt.h include/Nodes/Class.h include/Nodes/Function.h include/Nodes/Stmt/CompoundStmt.h include/Nodes/Expr/Expr.h include/Nodes/Expr/BinExpr.h include/Nodes/Expr/LiteralExpr.h include/Nodes/Includes.h src/Parser.cpp include/Nodes/Decls/VarDecl.h src/ParserError.cpp src/ParserError.h include/Nodes/Expr/IfExpr.h include/Nodes/Type.h include/Nodes/Expr/IdenExpr.h)

enable_testing()
add_executable(
        lexer_test
        tests/lexer_test.cpp
        src/Lexer.cpp
        include/Parser.h src/Parser.cpp src/ParserError.cpp src/ParserError.h)
add_executable(
        parser_test
        tests/parser_test.cpp
        src/Parser.cpp
        src/Lexer.cpp
        include/Parser.h src/Parser.cpp src/ParserError.cpp src/ParserError.h)

target_link_libraries(
        lexer_test
        gtest
        gtest_main
        pthread
)

target_link_libraries(
        parser_test
        gtest
        gtest_main
        pthread
)

include(GoogleTest)
gtest_discover_tests(lexer_test parser_test)
