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
#include "llvm/Support/raw_ostream.h"

#include "include/Codegen.h"
#include "include/Nodes/Program.h"
#include "include/Utility.h"

void Codegen::Visit(const Program &program)
{
    Function writeln("writeln", Type::INTEGER, {{"val", Type::INTEGER}});
    writeln.Accept(*this);
    llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context) ,false), 
        llvm::Function::ExternalLinkage, 
        "writeln",
        module);
    for(const auto &f : program.functions) {
        try {
            f.Accept(*this);
        } catch (const CodegenError &err) {
            llvm::errs() << "In function " << f.GetName() << "\n";
            ErrorReport(err);
        }
    }
    module.print(output, nullptr);
}
