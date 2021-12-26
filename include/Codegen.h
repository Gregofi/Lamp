#include <exception>
#include <string>
#include "include/Nodes/Visitor.h"

class CodegenError : public std::exception
{
 public:
    explicit CodegenError(std::string s) : message(std::move(s))  {}
    [[nodiscard]] const char *what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;

};

class Codegen : public Visitor
{
public:
    void Visit(const BinExpr &expr) override;
    void Visit(const LiteralExpr &expr) override;
    
    void Visit(const Function &function) override;
    void Visit(const Program &program) override;
};
