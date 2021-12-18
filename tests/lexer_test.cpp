#include "gtest/gtest.h"
#include "include/Lexer.h"
#include <iostream>
#include <sstream>

TEST(LexerTest, NumberParsingSimple)
{
    std::istringstream iss("1 2 12 014 0x14 0 00 0x0 1.5 0.5 0.0 1123.123 12.");
    Lexer lexer(iss);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 1);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 2);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 12);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 12);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 20);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 0);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 0);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 0);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 1.5);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 0.5);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 0.0);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 1123.123);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 12.0);
}

TEST(LexerTest, IdentifiersAndKeywordsTest)
{
    std::istringstream iss("Hello world1 _return return def def1 _ if a a1a");
    Lexer lexer(iss);
    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "Hello");

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "world1");

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "_return");

    EXPECT_EQ(lexer.GetNextToken(), Token::RETURN);
    EXPECT_EQ(lexer.GetNextToken(), Token::DEF);

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "def1");

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "_");

    EXPECT_EQ(lexer.GetNextToken(), Token::IF);

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "a");

    EXPECT_EQ(lexer.GetNextToken(), Token::IDENTIFIER);
    EXPECT_EQ(lexer.GetStringVal(), "a1a");
}

TEST(LexerTest, AE)
{
    std::istringstream iss("1 + 2.3 * 32 / 4.5 + 5 - 64");
    Lexer lexer(iss);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 1);

    EXPECT_EQ(lexer.GetNextToken(), Token::OP_PLUS);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 2.3);

    EXPECT_EQ(lexer.GetNextToken(), Token::OP_ASTERISK);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 32);

    EXPECT_EQ(lexer.GetNextToken(), Token::OP_DIVIDE);

    EXPECT_EQ(lexer.GetNextToken(), Token::FLOAT_LITERAL);
    EXPECT_EQ(lexer.GetDoubleVal(), 4.5);

    EXPECT_EQ(lexer.GetNextToken(), Token::OP_PLUS);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 5);

    EXPECT_EQ(lexer.GetNextToken(), Token::OP_MINUS);

    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetIntVal(), 64);
}

TEST(LexerTest, Brackets)
{
    std::istringstream iss("1 ( 1 ) ( 1 ) 1");
    Lexer lexer(iss);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::LBRACKET); 
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::RBRACKET);
    EXPECT_EQ(lexer.GetNextToken(), Token::LBRACKET);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::RBRACKET);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
}

TEST(LexerTest, If)
{
    std::istringstream iss("if(0) 1 + 2 else 2 + 3");
    Lexer lexer(iss);
    EXPECT_EQ(lexer.GetNextToken(), Token::IF);
    EXPECT_EQ(lexer.GetNextToken(), Token::LBRACKET);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::RBRACKET);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::OP_PLUS);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::ELSE);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
    EXPECT_EQ(lexer.GetNextToken(), Token::OP_PLUS);
    EXPECT_EQ(lexer.GetNextToken(), Token::INT_LITERAL);
}
