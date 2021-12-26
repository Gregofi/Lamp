#ifndef VISITOR_H
#define VISITOR_H

class BinExpr;
class LiteralExpr;
class ReturnExpr;
class IfExpr;
class CallExpr;
class CompoundExpr;
class IdenExpr;
class VarDecl;

class Function;
class Program;

class Visitor
{
public:
    virtual void Visit(const BinExpr &expr) = 0;
    virtual void Visit(const LiteralExpr &expr) = 0;
    virtual void Visit(const ReturnExpr &expr) = 0;
    virtual void Visit(const IfExpr &expr) = 0;
    virtual void Visit(const CallExpr &expr) = 0;
    virtual void Visit(const CompoundExpr &expr) = 0;
    virtual void Visit(const IdenExpr &expr) = 0;
    virtual void Visit(const VarDecl &expr) = 0;
    
    virtual void Visit(const Function &function) = 0;
    virtual void Visit(const Program &program) = 0;
};

#endif
