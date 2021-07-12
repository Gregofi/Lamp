#ifndef LAMP_LITERALEXPR_H
#define LAMP_LITERALEXPR_H

#include "include/Nodes/Expr/Expr.h"

class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(int val) : val(val) {}
protected:
    int val;
};

#endif //LAMP_LITERALEXPR_H
