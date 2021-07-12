#ifndef LAMP_COMPOUNDSTMT_H
#define LAMP_COMPOUNDSTMT_H

#include "include/Nodes/Stmt/Stmt.h"
#include <vector>

class CompoundStmt : public Stmt
{
public:
    explicit CompoundStmt(std::vector<std::unqiue_ptr<Stmt> > &&stmts) : stmts(std::move(stmts)) {}
private:
    std::vector<std::unqiue_ptr<Stmt> > stmts;
};

#endif //LAMP_COMPOUNDSTMT_H
