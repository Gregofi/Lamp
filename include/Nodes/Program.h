#ifndef LAMP_PROGRAM_H
#define LAMP_PROGRAM_H

#include <memory>
#include <vector>
#include <map>
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
    Program(std::map<std::string, Function> functions,
            std::map<std::string, std::unique_ptr<Class> > classes)
                    : functions(std::move(functions)), classes(std::move(classes))
    {

    }
    std::map<std::string, Function> functions;
    std::map<std::string, std::unique_ptr<Class> > classes;
    void Accept(Visitor &v) const { v.Visit(*this); }
};

#endif //LAMP_PROGRAM_H
