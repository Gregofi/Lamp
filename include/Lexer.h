#ifndef LAMP_LEXER_H
#define LAMP_LEXER_H

#include <string>
#include <iostream>
#include <map>
#include <optional>

#include "include/Utility.h"

#define POW2(x) (1 << (x))

enum class Token {
        /* Literals */
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        /* Data types */
        INT,
        DOUBLE,
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
        OP_ASSIGN,
        OP_GREATER,
        OP_LESS,
        /* Other */
        IDENTIFIER,
        LBRACKET,
        RBRACKET,
        LSQUAREB,
        RSQUAREB,
        LCURLYB,
        RCURLYB,
        IF,
        ELSE,
        DOUBLE_DOT,
        END_OF_INPUT,
        VAL,
        VAR,
        COMMA,
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
    bool IsEOF() const { return is.eof(); }
    void SignalError(std::string message); 
protected:
    Token ParseNumLiteral();
    Token ParseIdentifier();
    char FetchNext();
    bool IsCorrectDigit(char c, int base);
    std::optional<Token> ParseBraces() const;
    
    /* TODO : Put these guys in union or variant */
    char currChar{' '};
    int intVal{0};
    double floatVal{0.0};
    std::string stringVal{};
    std::istream &is;

    /* Position info */
    int posLine{1};
    int posChar{0};

    static const std::map<std::string, Token> keywords_map;
    static const std::map<std::string, Token> operators_map;

    std::optional<Token> ParseOperators();
};

#endif //LAMP_LEXER_H
