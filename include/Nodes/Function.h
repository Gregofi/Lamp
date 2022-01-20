#ifndef LAMP_FUNCTION_H
#define LAMP_FUNCTION_H

#include <memory>
#include <map>
#include <vector>
#include "include/Nodes/Expr/Expr.h"
#include "Decls/VarDecl.h"
#include "include/Nodes/Visitor.h"

struct Arg {
    std::string name;
    Type type;
};

/**
 * Represents function.
 */
class Function
{
public:
    Function(std::string name, Type ret_type, std::vector<Arg> &&arguments, std::unique_ptr<Expr> &&body = nullptr)
        : name(std::move(name)), ret_type(ret_type), arguments(std::move(arguments)), body(std::move(body))
    {

    }

    std::string GetName() const { return name; }
    Type GetReturnType() const { return ret_type; }
    const std::vector<Arg>& GetArguments() const { return arguments; }
    const std::unique_ptr<Expr>& GetBody() const { return body; }
    void Accept(Visitor &v) const { v.Visit(*this); }
    void SetBody(std::unique_ptr<Expr> &&other) { body = std::move(other); };
protected:
    std::string name;
    Type ret_type;
    std::vector<Arg> arguments;
    std::unique_ptr<Expr> body;
};

#endif //LAMP_FUNCTION_H
