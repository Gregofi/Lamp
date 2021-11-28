//
// Created by filip on 6/30/21.
//

#ifndef LAMP_PARSER_H
#define LAMP_PARSER_H

#include <include/Nodes/Type.h>
#include <cassert>
#include <utility>
#include <src/ParserError.h>
#include "include/Nodes/Includes.h"
#include "Lexer.h"

class Parser
{
public:
    Parser(std::istream &is) : lexer(is)
    {
        ReadNextToken();
    }
    Program ParseProgram();
    Function ParseFunction();
    std::unique_ptr<Stmt> ParseStmt();
    CompoundStmt ParseCompoundStmt();
    std::unique_ptr<Expr> ParseExpr();
    std::unique_ptr<Expr> ParseBinExpr();
protected:
    static Type MatchTypeToToken(Token token);
    static Operator MatchOperatorToToken(Token token);

    /**
     * Parses expression that may or may not be a left or right side of binary operator.
     * @return
     */
    std::unique_ptr<Expr> ParsePrimary();

    /**
     * Fetches next from input and checks if it is equal to 'token' arg. If not, throws parser error with error message
     * equal to 'error_message'.
     * @param token - Expected token
     * @param error_message - Error message to be displayed if token at input doesn't match
     * @throws ParserError - If token is not the same as input token
     */
    void FetchNextOrThrow(Token token, std::string error_message)
    {
        if(ReadNextToken() != token)
            throw ParserError(std::move(error_message));
    }
    /**
     * Returns binary operator precedence for given token (Higher number means lower precedence).
     * If token is not binary operator, then it returns INT_MAX.
     * @param token
     * @return precedence
     */
    static int GetTokenPrecedence(Token token);

    std::unique_ptr<Expr> ParseBinExprRHS(int precedence, std::unique_ptr<Expr> LHS);

    Token ReadNextToken()
    {
        return currTok = lexer.GetNextToken();
    }

    Token currTok;
    Lexer lexer;

};

#endif //LAMP_PARSER_H
