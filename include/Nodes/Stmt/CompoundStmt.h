#ifndef LAMP_COMPOUNDSTMT_H
#define LAMP_COMPOUNDSTMT_H

#include "include/Nodes/Stmt/Stmt.h"
#include <vector>
#include <memory>

class CompoundStmt : public Stmt
{
public:
    explicit CompoundStmt(std::vector<std::unique_ptr<Stmt> > &&stmts) : stmts(std::move(stmts)) {}
private:
    std::vector<std::unique_ptr<Stmt> > stmts;
};

#endif //LAMP_COMPOUNDSTMT_H
