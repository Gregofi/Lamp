

#include <include/Parser.h>

Program Parser::ParseProgram() {
    while(!lexer.IsEOF())
    {
        ReadNextToken();
        if(currTok == Token::DEF)
            ParseFunction();
        else if(currTok == Token::CLASS)
            ParseClass();
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

