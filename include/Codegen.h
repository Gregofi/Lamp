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
    Codegen() : builder(context) {}
    void Visit(const BinExpr &expr) override;
    void Visit(const LiteralExpr &expr) override;
    void Visit(const CallExpr &expr) override;
    void Visit(const Function &function) override;
    void Visit(const Program &program) override;
private:
    llvm::Value* OperatorsInt(llvm::Value* lhs, llvm::Value *rhs, Operator op);
    llvm::Function* GeneratePrototype(const Function &function);
    /* This represents return value, virtual functions can't be templated */
    llvm::Value *ret_value;
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    llvm::StringMap<llvm::Value *> named_values;
};
