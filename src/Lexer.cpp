#include "include/Lexer.h"
#include <cctype>
#include <cassert>
#include <optional>


const std::map<std::string, Token::Kind> Lexer::keywords_map = {
        {"def", Token::Kind::DEF},
        {"class", Token::Kind::CLASS},
        {"return", Token::Kind::RETURN},
        {"if", Token::Kind::IF},
        {"else", Token::Kind::ELSE},
        {"val", Token::Kind::VAL},
        {"var", Token::Kind::VAR},
        {"Int", Token::Kind::INT},
        {"Double", Token::Kind::DOUBLE},
};

const std::map<std::string, Token::Kind> Lexer::operators_map = {
        {"+", Token::Kind::OP_PLUS},
        {"-", Token::Kind::OP_MINUS},
        {"/", Token::Kind::OP_DIVIDE},
        {"*", Token::Kind::OP_ASTERISK},
        {"==", Token::Kind::OP_EQUAL},
};

Token Lexer::GetNextToken() {

    /* Ignore whitespaces */
    while(isspace(currChar))
    {
        FetchNext();
    }

    if(currChar == EOF)
        return CreateTok(Token::Kind::END_OF_INPUT);

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
        return CreateTok(Token::Kind::DOUBLE_DOT);
    }
    if(auto op = ParseOperators())
    {
        FetchNext();
        return *op;
    }

    if(currChar == ',')
    {
        FetchNext();
        return CreateTok(Token::Kind::COMMA);
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
            SignalError("Float numbers can only use base of 10.");
        }
        while(IsCorrectDigit(currChar, base))
        {
            num += currChar;
            FetchNext();
        }
        floatVal = strtod(num.c_str(), nullptr);
        return CreateTok(Token::Kind::FLOAT_LITERAL);
    }
    intVal = strtol(num.c_str(), nullptr, base);
    return CreateTok(Token::Kind::INT_LITERAL);
}

void Lexer::SignalError(std::string message) {
    std::cerr << posLine << ":" << posChar << ": " << "\u001b[31m" << "error: \u001b[0m" << message << "\n";
}

char Lexer::FetchNext() {
    ++ posChar; 
    currChar = is.get();
    if(currChar == '\n') {
        posChar = 0;
        ++ posLine;
    }
    return currChar;
}

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
    SignalError("Allowed bases are 2, 8, 10 and 16");
    return false;
}

Token Lexer::ParseIdentifier() {
    std::string s;
    if(isalpha(currChar) || currChar == '_')
    {
        s += currChar;
    }
    else
    {
        SignalError("Identifier has to start with a letter or _");
    }
    FetchNext();
    while(isalnum(currChar) || currChar == '_')
    {
        s += currChar;
        FetchNext();
    }
    if(keywords_map.count(s))
    {
        return CreateTok(keywords_map.at(s));
    }
    stringVal = s;
    return CreateTok(Token::Kind::IDENTIFIER);
}

std::optional<Token> Lexer::ParseBraces() const {
    switch(currChar)
    {
        case '(' : return CreateTok(Token::Kind::LBRACKET);
        case ')' : return CreateTok(Token::Kind::RBRACKET);
        case '{' : return CreateTok(Token::Kind::LCURLYB);
        case '}' : return CreateTok(Token::Kind::RCURLYB);
        case '[' : return CreateTok(Token::Kind::LSQUAREB);
        case ']' : return CreateTok(Token::Kind::RSQUAREB);
    }
    return std::nullopt;
}

std::optional<Token> Lexer::ParseOperators() {
    std::optional<Token> res;
    if(currChar == '+')
    {
        res = CreateTok(Token::Kind::OP_PLUS);
    }
    if(currChar == '-')
    {
        res = CreateTok(Token::Kind::OP_MINUS);
    }
    if(currChar == '*')
    {
        res = CreateTok(Token::Kind::OP_ASTERISK);
    }
    if(currChar == '/')
    {
        res = CreateTok(Token::Kind::OP_DIVIDE);
    }
    if(currChar == '=')
    {
        res = CreateTok(Token::Kind::OP_ASSIGN);
    }
    if(currChar == '>')
    {
        res = CreateTok(Token::Kind::OP_GREATER);
    }
    if(currChar == '<')
    {
        res = CreateTok(Token::Kind::OP_LESS);
    }
    return res;
}
