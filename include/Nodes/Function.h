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


protected:
    std::map<std::string, VarDecl> arguments;
    std::unique_ptr<Stmt> body;
};

#endif //LAMP_FUNCTION_H
