#ifndef LAMP_LEXER_H
#define LAMP_LEXER_H

#include <string>
#include <iostream>
#include <map>
#include <optional>

#include "include/Utility.h"


enum class Token {
        /* Literals */
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        /* Data types */
        INT,
        FLOAT,
        STRING,
        /* Other keywords */
        DEF,
        CLASS,
        RETURN,
        /* Operators */
        OP_EQUAL,
        OP_PLUS,
        OP_MINUS,
        OP_ASTERISK,
        OP_DIVIDE,
        /* Other */
        IDENTIFIER,
        LBRACKET,
        RBRACKET,
        LSQUAREB,
        RSQUAREB,
        LCURLYB,
        RCURLYB,
};

class LexerError : public MessageException
{
public:
    explicit LexerError(std::string message) : MessageException(std::move(message)) {}
};

class Lexer
{
public:
    explicit Lexer(std::istream &is) : is(is) {}
    Token GetNextToken();
    int GetIntVal() const { return intVal; }
    double GetDoubleVal() const { return floatVal; }
    std::string GetStringVal() const { return stringVal; }
protected:
    Token ParseNumLiteral();
    Token ParseIdentifier();
    char FetchNext();
    static bool IsCorrectDigit(char c, int base);
    std::optional<Token> ParseBraces() const;
    std::optional<Token> ParseOperators();

    Token currTok;
    char currChar{' '};
    int intVal{0};
    double floatVal{0.0};
    std::string stringVal{};
    std::istream &is;

    static const std::map<std::string, Token> keywords_map;
};

#endif //LAMP_LEXER_H
