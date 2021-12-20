#ifndef LAMP_FUNCTION_H
#define LAMP_FUNCTION_H

#include <memory>
#include <map>
#include "include/Nodes/Expr/Expr.h"
#include "Decls/VarDecl.h"

/**
 * Represents function.
 */
class Function
{
public:
    Function(std::string name, Type ret_type, std::map<std::string, Type> &&arguments, std::unique_ptr<Expr> &&body)
        : name(std::move(name)), ret_type(ret_type), arguments(std::move(arguments)), body(std::move(body))
    {

    }

    std::string GetName() const { return name; }
    Type GetReturnType() const { return ret_type; }
    const std::map<std::string, Type>& GetArguments() const { return arguments; }
    const Expr& GetBody() const { return *body; }
protected:
    std::string name;
    Type ret_type;
    std::map<std::string, Type> arguments;
    std::unique_ptr<Expr> body;
};

#endif //LAMP_FUNCTION_H
