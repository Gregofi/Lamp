#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "include/Visitor.h"
#include "include/Nodes/Expr/BinExpr.h"
#include "include/Nodes/Expr/LiteralExpr.h"
#include "include/Nodes/Expr/ReturnExpr.h"
#include "include/Nodes/Expr/IfExpr.h"
#include "include/Nodes/Expr/CallExpr.h"
#include "include/Nodes/Expr/CompoundExpr.h"
#include "include/Nodes/Expr/IdenExpr.h"
#include "include/Nodes/Decls/VarDecl.h"

/**
 * Is used for printing AST in human readable format.
 *
 */
class ASTPrinter : public Visitor
{
public:
    virtual void Visit(const BinExpr &expr)
    {
        PrintOffset();
        std::cout << "Binary expr" << std::endl;
        offset += 1;
        expr.GetRHS()->Accept(*this);
        expr.GetLHS()->Accept(*this);
        offset -= 1;
    }

    virtual void Visit(const LiteralExpr &expr)
    {
        PrintOffset();
        std::cout << "LiteralExpr; val: ";
        /* TODO : Value printing */
        std::cout << "\n";
    }

    virtual void Visit(const ReturnExpr &expr)
    {
        PrintOffset();
        std::cout << "ReturnExpr\n";
        offset += 1;
        expr.GetExpr().Accept(*this);
        offset -= 1;
    }

    virtual void Visit(const IfExpr &expr)
    {
        PrintOffset();
        std::cout << "IfExpr\n";
        offset += 1;
        expr.GetCond()->Accept(*this);
        expr.GetIfBody()->Accept(*this);
        expr.GetElseBody()->Accept(*this);
        offset -= 1;
    }
    
    virtual void Visit(const CallExpr &expr)
    {
        PrintOffset();
        std::cout << "CallExpr; callee: " << expr.callee << "\n";
    }

    virtual void Visit(const CompoundExpr &expr)
    {
        PrintOffset();
        std::cout << "CompoundExpr:\n";
        offset += 1;
        for(const auto &e : expr.GetExpressions()) {
            e->Accept(*this);
        }
        offset -= 1;
    }

    virtual void Visit(const IdenExpr &expr)
    {
        PrintOffset();
        std::cout << "IdentifierExpr: " << expr.GetName() << "\n";
    }

    virtual void Visit(const VarDecl &expr)
    {
        PrintOffset();
        std::cout << "VarDecl: ";
        std::cout << "{name: " << expr.GetName() << ", mutable: "
                  << expr.IsMutable() << "}\n";
        if(expr.GetValue()) {
            offset += 1;
            expr.GetValue()->Accept(*this);
            offset -= 1;
        }
    }
    
    virtual void Visit(const Function &function)
    {
        PrintOffset();
        std::cout << "Function: {name: " << function.GetName() << ", arguments{";
        /* TODO : Argument type printing */
        for(const auto &x : function.GetArguments()) {
            /* TODO : Don't print last comma */
            std::cout << x.name << ", ";
        }
        std::cout << "}}\n";
        offset += 1;
        function.GetBody()->Accept(*this);
        offset -= 1;
    }

    virtual void Visit(const Program &program)
    {
        PrintOffset();
        offset += 1;
        for(const auto &f : program.functions) {
            f.second.Accept(*this);
        }
        offset -= 1;
    }

protected:
    int offset = 0;
    void PrintOffset() const
    {
        std::cout << std::string(offset, ' ');
    }
};

#endif
