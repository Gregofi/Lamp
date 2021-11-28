#ifndef LAMP_FUNCTION_H
#define LAMP_FUNCTION_H

#include <memory>
#include <map>
#include "include/Nodes/Stmt/Stmt.h"
#include "Decls/VarDecl.h"

/**
 * Represents function.
 */
class Function
{
public:
    Function(std::string name, Type ret_type, std::map<std::string, Type> &&arguments, std::unique_ptr<Stmt> &&body)
        : name(std::move(name)), ret_type(ret_type), arguments(std::move(arguments)), body(std::move(body))
    {

    }

protected:
    std::string name;
    Type ret_type;
    std::map<std::string, Type> arguments;
    std::unique_ptr<Stmt> body;
};

#endif //LAMP_FUNCTION_H
