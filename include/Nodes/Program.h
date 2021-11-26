#ifndef LAMP_PROGRAM_H
#define LAMP_PROGRAM_H

#include <memory>
#include "include/Nodes/Node.h"
#include "include/Nodes/Stmt/Stmt.h"
#include "include/Nodes/Class.h"
#include "include/Nodes/Function.h"

/**
 * Represents the complete program, is on top of the hierarchy.
 */
class Program
{
public:
    Program(std::map<std::string, std::unique_ptr<Function> > funcs,
            std::map<std::string, std::unique_ptr<Class> > classes)
                    : functions(std::move(funcs)), classes(std::move(classes))
    {

    }
private:

    std::map<std::string, std::unique_ptr<Function> > functions;
    std::map<std::string, std::unique_ptr<Class> > classes;
};

#endif //LAMP_PROGRAM_H
