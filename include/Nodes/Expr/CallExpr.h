#ifndef CALLEXPR_H
#define CALLEXPR_H

#include "include/Nodes/Expr/Expr.h"
#include "include/Nodes/Decls/VarDecl.h"
#include "include/Nodes/Visitor.h"
#include <string>
#include <map>


class CallExpr : public Expr
{
public:
    CallExpr(std::string callee, std::map<std::string, std::unique_ptr<Expr> > arguments) :
        callee(std::move(callee)), arguments(std::move(arguments)) {}
    void Accept(Visitor &v) const override { v.Visit(*this); }
    std::string callee;
    std::map<std::string, std::unique_ptr<Expr> > arguments;
};
#endif
