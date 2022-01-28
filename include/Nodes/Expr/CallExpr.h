#ifndef CALLEXPR_H
#define CALLEXPR_H

#include "include/Nodes/Expr/Expr.h"
#include "include/Nodes/Decls/VarDecl.h"
#include "include/Token.h"
#include "include/Visitor.h"
#include <string>
#include <vector>


class CallExpr : public Expr
{
public:
    CallExpr(std::string callee, std::vector<std::unique_ptr<Expr> > arguments, SourceLocation loc) :
        Expr(loc), callee(std::move(callee)), arguments(std::move(arguments)) {}
    void Accept(Visitor &v) const override { v.Visit(*this); }
    std::string callee;
    std::vector<std::unique_ptr<Expr> > arguments;
};
#endif
