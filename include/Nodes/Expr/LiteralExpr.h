#ifndef LAMP_LITERALEXPR_H
#define LAMP_LITERALEXPR_H

#include <variant>
#include "include/Nodes/Expr/Expr.h"

class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(int val) : value(val) {}
    explicit LiteralExpr(double val) : value(val) {}
    const std::variant<int, double> &GetValue() const
    {
        return value;
    }
protected:
    std::variant<int, double> value;
};

#endif //LAMP_LITERALEXPR_H
