#include <exception>
#include <string>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/StringMap.h"

#include "include/Token.h"
#include "include/Visitor.h"
#include "include/Utility.h"

/** Sets ret_value of visitor to x and exits (calls return) function */
#define VIS_RETURN(x) { ret_value = (x); return; }
/* Calls accept of this visitor on object 'x' and returns value that was
   set in ret_value by the visit function */
#define VIS_ACCEPT(x) ((x)->Accept(*this), ret_value)

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
    Codegen() : builder(context), module("lamp codegen", context) {}
    void Visit(const BinExpr &expr) override;
    void Visit(const LiteralExpr &expr) override;
    void Visit(const CallExpr &expr) override;
    void Visit(const Function &function) override;
    void Visit(const Program &program) override;
    void Visit(const ReturnExpr &expr) override;
    void Visit(const CompoundExpr &expr) override;
    void Visit(const IdenExpr &expr) override;
    void Visit(const VarDecl &expr) override;
    void Visit(const IfExpr &expr) override;
private:
    void ErrorReport(const CodegenError &err)
    {
        std::cerr << fmt::format("{}:{} \u001b[31merror: \u001b[0m {}\n", err.loc.row, err.loc.column, err.message);
    }

    llvm::Value* OperatorsInt(llvm::Value* lhs, llvm::Value *rhs, Operator op);
    llvm::Function* GeneratePrototype(const Function &function);
    /* This represents Visit functions return value */
    llvm::Value *ret_value;
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    llvm::Module module;
    llvm::StringMap<llvm::Value *> named_values;

    llvm::raw_ostream &output;
};
