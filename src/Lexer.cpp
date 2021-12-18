#include "include/Lexer.h"
#include <cctype>
#include <cassert>
#include <optional>


const std::map<std::string, Token> Lexer::keywords_map = {
        {"def", Token::DEF},
        {"class", Token::CLASS},
        {"return", Token::RETURN},
        {"if", Token::IF},
        {"else", Token::ELSE},
};

const std::map<std::string, Token> Lexer::operators_map = {
        {"+", Token::OP_PLUS},
        {"-", Token::OP_MINUS},
        {"/", Token::OP_DIVIDE},
        {"*", Token::OP_ASTERISK},
        {"==", Token::OP_EQUAL},
};

Token Lexer::GetNextToken() {

    /* Ignore whitespaces */
    while(isspace(currChar))
    {
        FetchNext();
    }

    if(currChar == EOF)
        return Token::END_OF_INPUT;

        /* Handle numbers */
    if(isdigit(currChar))
    {
        return ParseNumLiteral();
    }

    if(auto braceTok = ParseBraces())
    {
        FetchNext();
        return *braceTok;
    }
    if(currChar == ':')
    {
        FetchNext();
        return Token::DOUBLE_DOT;
    }
    if(auto op = ParseOperators())
    {
        FetchNext();
        return *op;
    }

    /* At the end, parse keywords and identifiers */
    return ParseIdentifier();
}

Token Lexer::ParseNumLiteral() {
    int base = 10;
    if(currChar == '0')
    {
        FetchNext();
        if(currChar == 'x')
        {
            base = 16;
            FetchNext();
        }
        else if(currChar == 'b')
        {
            base = 2;
            FetchNext();
        }
        else
        {
            base = 8;
        }
    }

    std::string num;
    while(IsCorrectDigit(currChar, base))
    {
        num += currChar;
        FetchNext();
    }
    if(currChar == '.')
    {
        num += '.';
        FetchNext();
        if(base != 10 && base != 8)
        {
            throw LexerError("Float numbers can only use base of 10.");
        }
        while(IsCorrectDigit(currChar, base))
        {
            num += currChar;
            FetchNext();
        }
        floatVal = strtod(num.c_str(), nullptr);
        return Token::FLOAT_LITERAL;
    }
    intVal = strtol(num.c_str(), nullptr, base);
    return Token::INT_LITERAL;
}

char Lexer::FetchNext() { return currChar = is.get();  }

bool Lexer::IsCorrectDigit(char c, int base) {
    if(base == 10)
    {
        return isdigit(c);
    }
    if(base == 16)
    {
        return isxdigit(c);
    }
    if(base == 2)
    {
        return c == '0' || c == '1';
    }
    if(base == 8)
    {
        return c >= '0' && c <= '7';
    }
    assert(false && "Allowed bases are 2, 8, 10 and 16.");
}

Token Lexer::ParseIdentifier() {
    std::string s;
    if(isalpha(currChar) || currChar == '_')
    {
        s += currChar;
    }
    else
    {
        throw LexerError("This is not valid name for identifier");
    }
    FetchNext();
    while(isalnum(currChar) || currChar == '_')
    {
        s += currChar;
        FetchNext();
    }
    if(keywords_map.contains(s))
    {
        return keywords_map.at(s);
    }
    stringVal = s;
    return Token::IDENTIFIER;
}

std::optional<Token> Lexer::ParseBraces() const {
    switch(currChar)
    {
        case '(' : return Token::LBRACKET;
        case ')' : return Token::RBRACKET;
        case '{' : return Token::LCURLYB;
        case '}' : return Token::RCURLYB;
        case '[' : return Token::LSQUAREB;
        case ']' : return Token::RSQUAREB;
    }
    return std::nullopt;
}

std::optional<Token> Lexer::ParseOperators() {
    std::optional<Token> res;
    if(currChar == '+')
    {
        res = Token::OP_PLUS;
    }
    if(currChar == '-')
    {
        res = Token::OP_MINUS;
    }
    if(currChar == '*')
    {
        res = Token::OP_ASTERISK;
    }
    if(currChar == '/')
    {
        res = Token::OP_DIVIDE;
    }
    return res;
}

