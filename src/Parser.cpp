#include <climits>
#include <cassert>
#include "include/Parser.h"
#include "include/Nodes/Type.h"
#include "include/ParserError.h"
#include "include/Nodes/Expr/IdenExpr.h"
#include "include/Nodes/Expr/IfExpr.h"
#include "include/Nodes/Expr/CallExpr.h"
#include "include/Nodes/Expr/ReturnExpr.h"
#include "include/Nodes/Expr/CompoundExpr.h"
#include "include/Nodes/Decls/VarDecl.h"


Program Parser::ParseProgram()
{
    while(!lexer.IsEOF()) {
        try {
            if(currTok == Token::Kind::DEF) {
                auto func = ParseFunctionHead();
                auto &inserted_func = functions.emplace_back(std::move(func));
                if(currTok == Token::Kind::OP_ASSIGN) {
                    ReadNextToken();
                    auto body = ParseExpr();
                    inserted_func.SetBody(std::move(body));
                    if(inserted_func.GetName() == ENTRY_FUNCTION_NAME)
                        hasMain = true;
                }
            } else if(currTok == Token::Kind::CLASS) {
                throw std::runtime_error("Parsing classes is not yet implemented.");
            } else {
                throw ParserError("Expected either function or class definition", currTok.loc);
            }
        /* Error handling */
        } catch (const ParserError &err) {
            /* Report the error */
            ErrorReport(err);
            /* Eat tokens until a new function declaration or class declaration has been found */
            while(currTok != Token::Kind::DEF 
                    || currTok != Token::Kind::CLASS 
                    || currTok != Token::Kind::END_OF_INPUT)
                ReadNextToken();
        }
    }
    if(!hasMain)
        throw ParserError("No main function found", currTok.loc);
    return Program(std::move(functions), {});
}

Function Parser::ParseFunctionHead()
{
    if(ReadNextToken() != Token::Kind::IDENTIFIER) {
        throw ParserError("Expected an function name after 'def' keyword.", currTok.loc);
    }
    std::string fname = lexer.GetStringVal();
    FetchNextOrThrow(Token::Kind::LBRACKET, "Expected left bracket after function name");

    std::vector<Arg> args;
    /* Parse function params */
    ReadNextToken();
    while(true) {
        if(currTok == Token::Kind::RBRACKET)
            break;
        std::string arg_name = lexer.GetStringVal();
        FetchNextOrThrow(Token::Kind::DOUBLE_DOT, "Expected double dot after argument name");
        Type type = MatchTypeToToken(ReadNextToken());
        args.emplace_back(arg_name, type);
        ReadNextToken();
        if(currTok == Token::Kind::COMMA)
            FetchNextOrThrow(Token::Kind::IDENTIFIER, "Expected identifier after comma in argument list");
    }
    FetchNextOrThrow(Token::Kind::DOUBLE_DOT, "Expected ': ret_type' after argument list");
    auto type = MatchTypeToToken(ReadNextToken()); 
    ReadNextToken(); 
    return {std::move(fname), type, std::move(args)};
}

std::unique_ptr<Expr> Parser::ParseExpr()
{
    if (currTok == Token::Kind::IF) {
        return ParseIfExpr();
    } else if (currTok == Token::Kind::RETURN) {
        return ParseReturnExpr();
    } else if (currTok == Token::Kind::VAL || currTok == Token::Kind::VAR) {
        return ParseVarDecl();
    } else if (currTok == Token::Kind::LCURLYB) {
        return ParseCompoundExpr();
    } else {
        auto binexpr = ParseBinExpr();
        if (!binexpr) {
            throw ParserError("Unknown expression", currTok.loc);
        }
        return binexpr;
    } 
}

Type Parser::MatchTypeToToken(Token token)
{
    switch(token.kind)
    {
        case Token::Kind::INT : return Type::INTEGER;
        case Token::Kind::DOUBLE : return Type::DOUBLE;
        default : throw ParserError("Expected type", token.loc);
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

        LHS = std::make_unique<BinExpr>(std::move(LHS), std::move(RHS), MatchOperatorToToken(bin_op), currTok.loc);
    }
}

std::unique_ptr<Expr> Parser::ParseIfExpr() 
{
    assert(currTok == Token::Kind::IF);
         
    FetchNextOrThrow(Token::Kind::LBRACKET, "Expected opening bracket after 'if' keyword");   
    ReadNextToken(); 
    
    auto cond = ParseExpr();
    
    CheckCurrentAndGetNext(Token::Kind::RBRACKET, 
                           "Expected closing bracket after 'if' condition");
    
    auto body = ParseExpr();

    if(currTok == Token::Kind::ELSE)
    {
        ReadNextToken();
        auto else_body = ParseExpr();
        return std::make_unique<IfExpr>(std::move(cond), std::move(body), std::move(else_body), currTok.loc);
    }

    return std::make_unique<IfExpr>(std::move(cond), std::move(body), nullptr, currTok.loc);
}

std::unique_ptr<CallExpr> Parser::ParseFunctionCall(const std::string &name)
{
    std::vector<std::unique_ptr<Expr> > arguments;

    while(currTok != Token::Kind::RBRACKET) {
        arguments.emplace_back(ParseExpr());
        if(currTok != Token::Kind::COMMA)
            break;
        ReadNextToken();
    }
    
    CheckCurrentAndGetNext(Token::Kind::RBRACKET, "Expected closing bracket after arguments");

    return std::make_unique<CallExpr>(name, std::move(arguments), currTok.loc);
}

std::unique_ptr<ReturnExpr> Parser::ParseReturnExpr()
{
    assert(currTok == Token::Kind::RETURN);
    ReadNextToken();
    return std::make_unique<ReturnExpr>(ParseExpr(), currTok.loc);
}

std::unique_ptr<Expr> Parser::ParsePrimary()
{
    std::unique_ptr<Expr> result;
    if(currTok == Token::Kind::IDENTIFIER) {
        std::string id = lexer.GetStringVal();
        if(ReadNextToken() == Token::Kind::LBRACKET) {
            ReadNextToken();
            return ParseFunctionCall(id);
        } else {
            return std::make_unique<IdenExpr>(std::move(id), currTok.loc);
        }
    } else if(currTok == Token::Kind::INT_LITERAL) {
        result = std::make_unique<LiteralExpr>(lexer.GetIntVal(), currTok.loc);
    } else if(currTok == Token::Kind::FLOAT_LITERAL) {
        result = std::make_unique<LiteralExpr>(lexer.GetDoubleVal(), currTok.loc);
    }
    ReadNextToken();
    return result;
}

const static std::map<Token::Kind, int> precedences = {
        {Token::Kind::OP_MINUS, 40},
        {Token::Kind::OP_PLUS, 40},
        {Token::Kind::OP_ASTERISK, 20},
        {Token::Kind::OP_EQUAL, 60},
        {Token::Kind::OP_LESS, 50},
        {Token::Kind::OP_GREATER, 50},
        {Token::Kind::OP_DIVIDE, 20},
};

int Parser::GetTokenPrecedence(Token token)
{
    auto prec = precedences.find(token.kind);
    if(prec == precedences.end())
        return INT_MIN;
    return prec->second;
}

Operator Parser::MatchOperatorToToken(Token token)
{
    switch(token.kind) {
        case Token::Kind::OP_DIVIDE : return Operator::DIVIDE;
        case Token::Kind::OP_ASTERISK : return Operator::MULTIPLY;
        case Token::Kind::OP_EQUAL : return Operator::EQUAL;
        case Token::Kind::OP_PLUS : return Operator::PLUS;
        case Token::Kind::OP_MINUS : return Operator::MINUS;
        case Token::Kind::OP_LESS : return Operator::LESS;
        case Token::Kind::OP_GREATER : return Operator::GREATER;
        default: throw ParserError("Unknown operator", token.loc);
    }
}

std::unique_ptr<Stmt> Parser::ParseStmt()
{
    return nullptr;
}

std::unique_ptr<VarDecl> Parser::ParseVarDecl()
{
    assert(currTok == Token::Kind::VAL || currTok == Token::Kind::VAR);
    bool is_mutable = currTok == Token::Kind::VAR;

    FetchNextOrThrow(Token::Kind::IDENTIFIER, "Expected an identifier after 'val' keyword");
    std::string name = lexer.GetStringVal();

    FetchNextOrThrow(Token::Kind::DOUBLE_DOT, "Expected an : DATA_TYPE after val name");
    ReadNextToken();
    auto type = MatchTypeToToken(currTok);

    ReadNextToken();
    std::unique_ptr<Expr> value;
    if(currTok == Token::Kind::OP_ASSIGN) {
        ReadNextToken();
        value = ParseExpr(); 
    }
    
    return std::make_unique<VarDecl>(std::move(name), type, is_mutable, std::move(value), currTok.loc);
}

std::unique_ptr<CompoundExpr> Parser::ParseCompoundExpr()
{
    assert(currTok == Token::Kind::LCURLYB);
    ReadNextToken();
    std::vector<std::unique_ptr<Expr> > exprs;
    while(currTok != Token::Kind::RCURLYB) {
        exprs.emplace_back(ParseExpr());
    }
    CheckCurrentAndGetNext(Token::Kind::RCURLYB, "Expected closing curly bracket at the end of compound expr");
    return std::make_unique<CompoundExpr>(std::move(exprs), currTok.loc);
}
