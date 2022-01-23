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

#include <iostream>

#include "include/Codegen.h"
#include "include/Nodes/Function.h"
#include "include/Nodes/Type.h"

llvm::Function* Codegen::GeneratePrototype(const Function &function) {
    std::vector<llvm::Type*> types(function.GetArguments().size());
    for(const auto &arg : function.GetArguments()) {
        types.emplace_back(arg.type == Type::INTEGER 
                        ? llvm::Type::getInt32Ty(context) 
                        : llvm::Type::getFloatTy(context));
    }

    llvm::FunctionType *ft = llvm::FunctionType::get(
        function.GetReturnType() == Type::INTEGER 
                ? llvm::Type::getInt32Ty(context)
                : llvm::Type::getDoubleTy(context), types, false);

    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, 
                                               function.GetName(), module.get());

    size_t i = 0;
    const auto &args = function.GetArguments();
    for(auto &arg : f->args())
        arg.setName(args[i].name);

    return f;    
}

/** TODO : Fix if extern declaration doesn't match arguments of definition */
void Codegen::Visit(const Function &function)
{
    llvm::Function *gen_f = module->getFunction(function.GetName());

    if(!gen_f)
        gen_f = GeneratePrototype(function);
        
    if(!gen_f->empty()) {
        std::cerr << "Function cannot be redefined!";
        VIS_RETURN(nullptr);
    }

    /* This is forward decl */
    if(!function.GetBody())
        VIS_RETURN(gen_f);

    /* Body generation */
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(context, "entry", gen_f);
    builder.SetInsertPoint(BB);

    named_values.clear();
    for(auto &arg : gen_f->args())
        named_values[arg.getName()] = &arg;

    /* TODO : Fix this return value */
    if(llvm::Value* retval = VIS_ACCEPT(function.GetBody())) {
        builder.CreateRet(retval);
        llvm::verifyFunction(*gen_f);
        VIS_RETURN(gen_f);
    }

    /* Error reading body */
    gen_f->eraseFromParent();
    VIS_RETURN(nullptr);

}
