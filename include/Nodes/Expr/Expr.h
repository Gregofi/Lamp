#ifndef LAMP_EXPR_H
#define LAMP_EXPR_H

#include "include/Nodes/Node.h"
#include "include/Visitor.h"
#include "include/Token.h"

class Expr : public Node {
public:
    Expr(SourceLocation loc) : Node(loc) {}
    virtual void Accept(Visitor &v) const = 0;
};


#endif //LAMP_EXPR_H
