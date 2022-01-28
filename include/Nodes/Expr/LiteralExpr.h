#ifndef LAMP_LITERALEXPR_H
#define LAMP_LITERALEXPR_H

#include <variant>
#include "include/Nodes/Expr/Expr.h"
#include "include/Token.h"
#include "include/Visitor.h"

class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(int val, SourceLocation loc) : Expr(loc), value(val) {}
    explicit LiteralExpr(double val, SourceLocation loc) : Expr(loc), value(val) {}
    const std::variant<int, double> &GetValue() const
    {
        return value;
    }
    void Accept(Visitor &v) const { v.Visit(*this); }
protected:
    std::variant<int, double> value;
};

#endif //LAMP_LITERALEXPR_H
