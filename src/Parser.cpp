

#include <cassert>

#include "include/Parser.h"
#include "include/Nodes/Type.h"
#include "include/Nodes/Expr/IdenExpr.h"
#include "ParserError.h"
#include "include/Nodes/Expr/IfExpr.h"
#include "include/Nodes/Decls/VarDecl.h"

Program Parser::ParseProgram()
{
    std::vector<Function> functions;
    while(!lexer.IsEOF()) {
        if(currTok == Token::DEF) {
            ReadNextToken();
            functions.emplace_back(std::move(ParseFunction()));
        } else if(currTok == Token::CLASS) {
            throw std::runtime_error("Parsing classes is not yet implemented.");
        } else {
            throw ParserError("Expected either function or class definition");
        }
    }
    return Program(std::move(functions), {});
}

Function Parser::ParseFunction()
{
    assert(currTok == Token::DEF);
    if(ReadNextToken() != Token::IDENTIFIER) {
        throw ParserError("Expected an function name after 'def' keyword.");
    }
    std::string fname = lexer.GetStringVal();
    /* If this is present, then there is also a function body. Otherwise its forward declaration */
    FetchNextOrThrow(Token::LBRACKET, "Expected left bracket after function name");

    std::map<std::string, Type> args;
    /* Parse function params */
    ReadNextToken();
    while(true) {
        if(currTok == Token::RBRACKET)
            break;
        std::string arg_name = lexer.GetStringVal();
        FetchNextOrThrow(Token::DOUBLE_DOT, "Expected double dot after argument name");
        auto type = MatchTypeToToken(ReadNextToken());
        args[arg_name] = type;
        ReadNextToken();
        if(currTok == Token::COMMA) {
            FetchNextOrThrow(Token::IDENTIFIER, "Expected identifier after comma in argument list");    
        }
    }
    FetchNextOrThrow(Token::DOUBLE_DOT, "Expected ': ret_type' after argument list");
    auto type = MatchTypeToToken(ReadNextToken()); 
    FetchNextOrThrow(Token::OP_ASSIGN, "Expected = before function body");
    ReadNextToken();
    auto body = ParseExpr();
    return {std::move(fname), type, std::move(args), std::move(body)};
}

std::unique_ptr<Expr> Parser::ParseExpr()
{
    if (currTok == Token::IF) {
        return ParseIfExpr();
    } else {
        auto binexpr = ParseBinExpr();
        if (!binexpr) {
            throw ParserError("Unknown expression");
        }
        return binexpr;
    }
}

Type Parser::MatchTypeToToken(Token token)
{
    switch(token)
    {
        case Token::INT : return Type::INTEGER;
        case Token::DOUBLE : return Type::DOUBLE;
        default : throw ParserError("Expected type");
    }
}

std::unique_ptr<Expr> Parser::ParseBinExpr()
{
    auto LHS = ParsePrimary();
    if(!LHS)
        return NULL;

    return ParseBinExprRHS(0, std::move(LHS));
}

std::unique_ptr<Expr> Parser::ParseBinExprRHS(int precedence, std::unique_ptr<Expr> LHS)
{
    while(true) {
        int tok_precedence = GetTokenPrecedence(currTok);

        /* If precedence of current op is higher than our expressions, or
         * current token is not operator at all then we are done. */
        if (tok_precedence < precedence)
            return LHS;

        Token bin_op = currTok;
        ReadNextToken(); // eat binop

        auto RHS = ParsePrimary();
        if(!RHS)
            return nullptr;

        int next_precedence = GetTokenPrecedence(currTok);
        if (tok_precedence < next_precedence) {
            RHS = ParseBinExprRHS(tok_precedence + 1, std::move(RHS));
            if(!RHS)
                return nullptr;
        }

        LHS = std::make_unique<BinExpr>(std::move(LHS), std::move(RHS), MatchOperatorToToken(bin_op));
    }
}

std::unique_ptr<Expr> Parser::ParseIfExpr() 
{
    assert(currTok == Token::IF);
         
    FetchNextOrThrow(Token::LBRACKET, "Expected opening bracket after 'if' keyword");   
    ReadNextToken(); 
    
    auto cond = ParseExpr();
    
    CheckCurrentAndGetNext(Token::RBRACKET, 
                           "Expected closing bracket after 'if' condition");
    
    auto body = ParseExpr();

    if(currTok == Token::ELSE)
    {
        ReadNextToken();
        auto else_body = ParseExpr();
        return std::make_unique<IfExpr>(std::move(cond), std::move(body), std::move(else_body));
    }

    return std::make_unique<IfExpr>(std::move(cond), std::move(body));
}

std::unique_ptr<Expr> Parser::ParsePrimary()
{
    std::unique_ptr<Expr> result;
    if(currTok == Token::IDENTIFIER) {
        result = std::make_unique<IdenExpr>(lexer.GetStringVal());
    } else if(currTok == Token::INT_LITERAL) {
        result = std::make_unique<LiteralExpr>(lexer.GetIntVal());
    } else if(currTok == Token::FLOAT_LITERAL) {
        result = std::make_unique<LiteralExpr>(lexer.GetDoubleVal());
    }
    ReadNextToken();
    return result;
}

const static std::map<Token, int> precedences = {
        {Token::OP_MINUS, 40},
        {Token::OP_PLUS, 40},
        {Token::OP_ASTERISK, 20},
        {Token::OP_EQUAL, 60},
        {Token::OP_DIVIDE, 20},
};

int Parser::GetTokenPrecedence(Token token)
{
    auto prec = precedences.find(token);
    if(prec == precedences.end())
        return INT_MIN;
    return prec->second;
}

Operator Parser::MatchOperatorToToken(Token token)
{
    switch(token) {
        case Token::OP_DIVIDE : return Operator::DIVIDE;
        case Token::OP_ASTERISK : return Operator::MULTIPLY;
        case Token::OP_EQUAL : return Operator::EQUAL;
        case Token::OP_PLUS : return Operator::PLUS;
        case Token::OP_MINUS : return Operator::MINUS;
        default: throw ParserError("Unknown operator");
    }
}

std::unique_ptr<Stmt> Parser::ParseStmt()
{
    return nullptr;
}

std::unique_ptr<VarDecl> Parser::ParseVarDecl()
{
    assert(currTok == Token::VAL || currTok == Token::VAR);
    bool is_mutable = currTok == Token::VAR;

    FetchNextOrThrow(Token::IDENTIFIER, "Expected an identifier after 'val' keyword");
    std::string name = lexer.GetStringVal();

    FetchNextOrThrow(Token::DOUBLE_DOT, "Expected an : DATA_TYPE after val name");
    ReadNextToken();
    auto type = MatchTypeToToken(currTok);

    ReadNextToken();
    std::unique_ptr<Expr> value;
    if(currTok == Token::OP_ASSIGN) {
        ReadNextToken();
        value = ParseExpr(); 
    }
    
    return std::make_unique<VarDecl>(std::move(name), type, is_mutable, std::move(value));
}
