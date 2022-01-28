#ifndef LAMP_IDENEXPR_H
#define LAMP_IDENEXPR_H

#include <string>

#include "Expr.h"
#include "include/Visitor.h"

class IdenExpr : public Expr
{
public:
    IdenExpr(std::string name, SourceLocation loc) : Expr(loc), name(std::move(name)) {}
    void Accept(Visitor &v) const override { v.Visit(*this); }
    std::string GetName() const { return name; }
private:
    std::string name;
};

#endif //LAMP_IDENEXPR_H
