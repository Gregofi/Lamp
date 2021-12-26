#ifndef LAMP_EXPR_H
#define LAMP_EXPR_H

#include "include/Nodes/Stmt/Stmt.h"
#include "include/Nodes/Visitor.h"

class Expr : public Stmt {
public:
    virtual void Accept(Visitor &v) const = 0;
};


#endif //LAMP_EXPR_H
