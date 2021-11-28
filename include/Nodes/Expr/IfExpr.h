//
// Created by gregofi1 on 11/26/21.
//

#ifndef LAMP_IFEXPR_H
#define LAMP_IFEXPR_H

#include "Expr.h"
#include <memory>
#include "include/Nodes/Stmt/Stmt.h"

class IfExpr : public Expr
{
public:
    IfExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> if_body, std::unique_ptr<Stmt> else_body)
                    : cond(std::move(cond)), if_body(std::move(if_body)), else_body(std::move(else_body))
    {

    }

private:
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Stmt> if_body;
    std::unique_ptr<Stmt> else_body;
};

#endif //LAMP_IFEXPR_H
