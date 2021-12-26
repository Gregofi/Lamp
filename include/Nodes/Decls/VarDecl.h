#ifndef LAMP_VARDECL_H
#define LAMP_VARDECL_H

#include <string>
#include <memory>
#include "include/Nodes/Type.h"
#include "include/Nodes/Expr/Expr.h"

class VarDecl : public Expr
{
public:
    VarDecl(std::string name, Type type, bool is_mutable, std::unique_ptr<Expr> value = nullptr) 
        : name(std::move(name)), type(type), 
          is_mutable(is_mutable), value(std::move(value)) {}
    std::string GetName() const { return name; }
    Type GetType() const { return type; }
    bool IsMutable() const { return is_mutable; }
    const std::unique_ptr<Expr>& GetValue() const { return value; }
    void Accept(Visitor &v) const override { v.Visit(*this); }
private:
    std::string name;
    Type type;
    bool is_mutable;
    std::unique_ptr<Expr> value;
};

#endif //LAMP_VARDECL_H
