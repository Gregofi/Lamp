#ifndef LAMP_IDENEXPR_H
#define LAMP_IDENEXPR_H

#include "Expr.h"
#include "include/Nodes/Visitor.h"

class IdenExpr : public Expr
{
public:
    IdenExpr(std::string name) : name(std::move(name)) {}
    void Accept(Visitor &v) const override { v.Visit(*this); }
private:
    std::string name;
};

#endif //LAMP_IDENEXPR_H
