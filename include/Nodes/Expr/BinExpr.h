

#ifndef LAMP_BINEXPR_H
#define LAMP_BINEXPR_H

#include <memory>
#include "include/Utility.h"
#include "include/Nodes/Expr/Expr.h"
#include "include/Visitor.h"

class BinExpr : public Expr
{
public:
    BinExpr(std::unique_ptr<Expr> LHS, std::unique_ptr<Expr> RHS, Operator op) 
                : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}
    const std::unique_ptr<Expr>& GetLHS() const { return LHS; }
    const std::unique_ptr<Expr>& GetRHS() const { return RHS; }
    Operator GetOp() const { return op; }
    void Accept(Visitor &v) const override { v.Visit(*this); }
protected:
    std::unique_ptr<Expr> LHS;
    std::unique_ptr<Expr> RHS;
    Operator op;
};

#endif //LAMP_BINEXPR_H
