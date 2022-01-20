#ifndef RETURNEXPR_H
#define RETURNEXPR_H

#include <memory>
#include "include/Nodes/Expr/Expr.h"
#include "include/Visitor.h"

class ReturnExpr : public Expr
{
public:
    ReturnExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
    const Expr& GetExpr() const { return *expr; }
    void Accept(Visitor &v) const override { v.Visit(*this); }
protected:
    std::unique_ptr<Expr> expr;
};

#endif
