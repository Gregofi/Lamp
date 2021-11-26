

#include <include/Parser.h>
#include "ParserError.h"
#include "include/Nodes/Expr/IfExpr.h"

Program Parser::ParseProgram() {
    while(!lexer.IsEOF())
    {
        ReadNextToken();
        if(currTok == Token::DEF)
            ParseFunction();
        else if(currTok == Token::CLASS)
            throw std::runtime_error("Parsing classes is not yet implemented.");
    }
}

Function Parser::ParseFunction() {
    ParseFunctionHead();
    /* If this is present, then there is also a function body. Otherwise its forward declaration */
    if(ReadNextToken() == Token::OP_EQUAL)
    {
        ReadNextToken();
        ParseStmt();

    }
}

Function Parser::ParseFunctionHead() {
    return Function();
}

std::unique_ptr<Expr> Parser::ParseExpr() {
    if(currTok == Token::IF)
    {
        if(ReadNextToken() != Token::LBRACKET)
            throw ParserError("Expected opening bracket after 'if' keyword");

        auto cond = ParseExpr();
        if(ReadNextToken() != Token::RBRACKET)
            throw ParserError("Expected closing bracket after if condition");

        auto if_body = ParseStmt();

        if(currTok == Token::ELSE)
        {
            ReadNextToken();
            auto else_body = ParseStmt();
            return std::make_unique<IfExpr>(std::move(cond), std::move(if_body), std::move(else_body));
        }
    }
}

