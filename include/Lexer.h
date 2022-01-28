#ifndef LAMP_LEXER_H
#define LAMP_LEXER_H

#include <string>
#include <iostream>
#include <map>
#include <optional>

#include "include/Utility.h"
#include "include/Token.h"

#define POW2(x) (1 << (x))


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
    Token CreateTok(Token::Kind kind) const
    {
        return Token(kind, posLine, posChar);
    }

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

    static const std::map<std::string, Token::Kind> keywords_map;
    static const std::map<std::string, Token::Kind> operators_map;

    std::optional<Token> ParseOperators();
};

#endif //LAMP_LEXER_H
