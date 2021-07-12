#ifndef LAMP_PROGRAM_H
#define LAMP_PROGRAM_H

#include <memory>
#include "include/Nodes/Node.h"
#include "include/Nodes/Stmt/Stmt.h"
#include "include/Nodes/Class.h"
#include "include/Nodes/Function.h"


class Program
{
public:
    explicit Program(std::unique_ptr<Stmt> body) : body(std::move(body)) {}
private:
    std::map<std::string, std::unique_ptr<Function> > funcs;
    std::map<std::string, std::unique_ptr<Class> > classes;
};

#endif //LAMP_PROGRAM_H
