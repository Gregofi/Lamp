#include <exception>
#include <string>

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

}

class Codegen : public Visitor
{
public:
    void visit(const BinExpr &expr);
    void visit(const Function &function);
    void visit(const LiteralExpr &expr);
};
