#ifndef COUMPOUNDEXPR_H
#define COUMPOUNDEXPR_H

#include "include/Nodes/Expr/Expr.h"
#include "include/Nodes/Visitor.h"
#include <vector>

class CompoundExpr : public Expr
{
public:
    CompoundExpr(std::vector<std::unique_ptr<Expr> > exprs) : exprs(std::move(exprs)) {}
    const std::vector<std::unique_ptr<Expr> >& GetExpressions() const { return exprs; };
    void Accept(Visitor &v) const override { v.Visit(*this); }
protected:
    std::vector<std::unique_ptr<Expr> > exprs;
};

#endif  
