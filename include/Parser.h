//
// Created by filip on 6/30/21.
//

#ifndef LAMP_PARSER_H
#define LAMP_PARSER_H

#include "include/Nodes/Includes.h"
#include "Lexer.h"

class Parser
{
public:
    Program ParseProgram();
protected:
    [[ nodiscard ]]
    Function ParseFunction();
    [[ nodiscard ]]
    Class ParseClass();
    [[ nodiscard ]]
    Stmt ParseStmt();
    [[ nodiscard ]]
    CompoundStmt ParseCompoundStmt();
    [[ nodiscard ]]
    Expr ParseExpr();
    [[ nodiscard ]]
    BinExpr ParseBinExpr();
    [[ nodiscard ]]
    LiteralExpr ParseLiteralExpr();

    Token ReadNextToken()
    {
        return currTok = lexer.GetNextToken();
    }
    Token currTok;
    Lexer lexer;

    Function ParseFunctionHead();
};

#endif //LAMP_PARSER_H
