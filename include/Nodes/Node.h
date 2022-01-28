#ifndef LAMP_NODE_H
#define LAMP_NODE_H

#include "include/Token.h"

class Node
{
public:
    Node(SourceLocation loc) : loc(loc) {}
    virtual ~Node() = default;
    SourceLocation GetLocation() const { return loc; }
protected:
    SourceLocation loc;
};

#endif //LAMP_NODE_H
