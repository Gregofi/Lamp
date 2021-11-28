//
// Created by gregofi1 on 11/28/21.
//

#ifndef LAMP_IDENEXPR_H
#define LAMP_IDENEXPR_H

#include "Expr.h"

class IdenExpr : public Expr
{
public:
    IdenExpr(std::string name) : name(std::move(name)) {}
private:
    std::string name;
};

#endif //LAMP_IDENEXPR_H
