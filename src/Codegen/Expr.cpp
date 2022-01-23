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

#include <variant>
#include <iostream>

#include "include/Codegen.h"
#include "include/Nodes/Expr/LiteralExpr.h"
#include "include/Nodes/Expr/BinExpr.h"
#include "include/Nodes/Expr/CallExpr.h"
#include "include/Utility.h"

using namespace llvm;

Value* Codegen::OperatorsInt(Value* lhs, Value *rhs, ::Operator op)
{
    switch(op) {
        case ::Operator::PLUS:
            return builder.CreateAdd(lhs, rhs, "addtmp");            
        case ::Operator::MINUS:
            return builder.CreateSub(lhs, rhs, "subtmp");
        case ::Operator::DIVIDE:
            return builder.CreateUDiv(lhs, rhs, "divtmp");
        case ::Operator::MULTIPLY:
            return builder.CreateMul(lhs, rhs, "mult");
        case ::Operator::EQUAL:
            return builder.CreateICmpEQ(lhs, rhs, "equaltmp");
        case ::Operator::ASSIGN:
            std::cerr << "Not implemented operator '='!" << std::endl;
            return nullptr; /* TODO */
        case ::Operator::AND:
            return builder.CreateAnd(lhs, rhs, "andtmp");
        case ::Operator::OR:
            return builder.CreateOr(lhs, rhs, "ortmp");
        case ::Operator::GREATER:
            return builder.CreateICmpUGT(lhs, rhs, "greatertmp");
        case ::Operator::LESS:
            return builder.CreateICmpSLT(lhs, rhs, "lesstmp");
        default:
            std::cerr << "Not implemented operator!" << std::endl;
            return nullptr; /* TODO */
        }

}

void Codegen::Visit(const BinExpr &expr)
{
    Value *lhs = VIS_ACCEPT(expr.GetLHS()); 
    Value *rhs = VIS_ACCEPT(expr.GetRHS());

    if(!lhs || !rhs)
        VIS_RETURN(nullptr);

    if(rhs->getType() != lhs->getType()) {
        /* TODO : Throw an error here */
        VIS_RETURN(nullptr);
    }

    /* Integer operators are only implemented for now */
    if(rhs->getType()->isFloatTy() || lhs->getType()->isFloatTy()) {
        std::cerr << "Floating point operations not yet supported" << std::endl;
        VIS_RETURN(nullptr);
    }
}  


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void Codegen::Visit(const LiteralExpr &expr)
{
    ret_value = std::visit(overloaded {
            [this](int arg) -> Value* { return ConstantInt::get(context, APInt(sizeof(arg), arg, true)); },
            [this](double arg) -> Value* { return ConstantFP::get(context, APFloat(arg)); },
        }, expr.GetValue());
}


void Codegen::Visit(const CallExpr &expr)
{
    llvm::Function *callee = module->getFunction(expr.callee);
    if(!callee) {
        std::cerr << "Unknown function referenced" << std::endl;
        VIS_RETURN(nullptr);
    }

    if(callee->arg_size() != expr.arguments.size()) {
        std::cerr << "Wrong number of arguments passed" << std::endl;
        VIS_RETURN(nullptr);
    }

    std::vector<Value*> arg_v;
    for(const auto &arg : expr.arguments) {
        arg_v.emplace_back(VIS_ACCEPT(arg.second));
        if(!arg_v.back())
            VIS_RETURN(nullptr);
    }

    VIS_RETURN(builder.CreateCall(callee, arg_v, "calltmp"));
}
