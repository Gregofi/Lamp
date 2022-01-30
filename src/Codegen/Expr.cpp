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
#include "llvm/IR/Instructions.h"

#include <fmt/format.h>

#include <variant>
#include <iostream>

#include "include/Codegen.h"
#include "include/Nodes/Expr/CompoundExpr.h"
#include "include/Nodes/Expr/LiteralExpr.h"
#include "include/Nodes/Expr/ReturnExpr.h"
#include "include/Nodes/Expr/CompoundExpr.h"
#include "include/Nodes/Decls/VarDecl.h"
#include "include/Nodes/Expr/BinExpr.h"
#include "include/Nodes/Expr/IfExpr.h"
#include "include/Nodes/Expr/CallExpr.h"
#include "include/Nodes/Expr/IdenExpr.h"
#include "include/Utility.h"
#include "include/Visitor.h"

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
            return builder.CreateIntCast(builder.CreateICmpEQ(lhs, rhs, "greatertmp"), 
                                         llvm::Type::getInt32Ty(context), false);
        case ::Operator::ASSIGN:
            std::cerr << "Not implemented operator '='!" << std::endl;
            return nullptr; /* TODO */
        case ::Operator::AND:
            return builder.CreateAnd(lhs, rhs, "andtmp");
        case ::Operator::OR:
            return builder.CreateOr(lhs, rhs, "ortmp");
        case ::Operator::GREATER:
            return builder.CreateIntCast(builder.CreateICmpSGE(lhs, rhs, "greatertmp"), 
                                         llvm::Type::getInt32Ty(context), false);
        case ::Operator::LESS:
            return builder.CreateIntCast(builder.CreateICmpSLT(lhs, rhs, "lesstmp"), 
                                        llvm::Type::getInt32Ty(context), false);
        default:
            throw CodegenError("Operator is not yet implemented '", {0, 0});
            return nullptr; /* TODO */
        }

}

void Codegen::Visit(const BinExpr &expr)
{
    Value *lhs = VIS_ACCEPT(expr.GetLHS()); 
    Value *rhs = VIS_ACCEPT(expr.GetRHS());

    if(!lhs || !rhs) {
        std::cerr << "Either left or right side of binexpr is invalid" << std::endl;
        VIS_RETURN(nullptr);
    }

    if(rhs->getType() != lhs->getType()) {
        /* TODO : Do something here, either cast or throw an error */

        std::cerr << "BinExpr types are not the same" << std::endl;
        lhs->getType()->print(llvm::errs());
        llvm::errs() << "\n";
        rhs->getType()->print(llvm::errs());
        llvm::errs() << "\n";
        VIS_RETURN(nullptr);
    }

    /* Integer operators are only implemented for now */
    if(rhs->getType()->isFloatTy() || lhs->getType()->isFloatTy()) {
        throw CodegenError(fmt::format("Floating point operations are not yet supported"), expr.GetLocation());
    }

    VIS_RETURN(OperatorsInt(lhs, rhs, expr.GetOp()));
}  


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void Codegen::Visit(const LiteralExpr &expr)
{
    ret_value = std::visit(overloaded {
            [this](int arg) -> Value* { return ConstantInt::get(context, APInt(sizeof(arg) * 8, arg, true)); },
            [this](double arg) -> Value* { return ConstantFP::get(context, APFloat(arg)); },
        }, expr.GetValue());
}


void Codegen::Visit(const CallExpr &expr)
{
    llvm::Function *callee = module.getFunction(expr.callee);
    if(!callee) {
        throw CodegenError(fmt::format("Function called here '{}' is unknown", expr.callee), expr.GetLocation());
    }

    if(callee->arg_size() != expr.arguments.size()) {
        throw CodegenError(fmt::format("Function '{}' requires {} arguments, {} given", 
                                        expr.callee, callee->arg_size(), expr.arguments.size()), expr.GetLocation());
    }

    std::vector<Value*> arg_v;
    for(const auto &arg : expr.arguments) {
        arg_v.emplace_back(VIS_ACCEPT(arg));
        if(!arg_v.back()) {
            std::cerr << "Error while parsing callexpr arguments" << std::endl;
            VIS_RETURN(nullptr);
        }
    }

    VIS_RETURN(builder.CreateCall(callee, arg_v, "calltmp"));
}


void Codegen::Visit(const IfExpr &expr)
{
    llvm::Value *cond_v = VIS_ACCEPT(expr.GetCond());
    if(!cond_v) {
        std::cerr << "Couldn't parse if condition" << std::endl;
        VIS_RETURN(nullptr);
    }

    cond_v = builder.CreateICmpNE(cond_v, ConstantInt::get(context, APInt(32, 0, true)), "ifcmp");

    llvm::Function *f = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBB = BasicBlock::Create(context, "then", f);
    llvm::BasicBlock *elseBB = BasicBlock::Create(context, "else");
    llvm::BasicBlock *mergeBB = BasicBlock::Create(context, "ifcont");

    builder.CreateCondBr(cond_v, thenBB, elseBB);

    builder.SetInsertPoint(thenBB);

    Value *then_v = VIS_ACCEPT(expr.GetIfBody());
    if(!then_v) {
        std::cerr << "Couldn't parse IfBody" << std::endl;
        VIS_RETURN(nullptr);
    }

    builder.CreateBr(mergeBB);

    /* This is required, because nested calls may change the actual block */
    thenBB = builder.GetInsertBlock();

    f->getBasicBlockList().push_back(elseBB);
    builder.SetInsertPoint(elseBB);

    llvm::Value *else_v = VIS_ACCEPT(expr.GetElseBody());

    builder.CreateBr(mergeBB);
    elseBB = builder.GetInsertBlock();

    f->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    llvm::PHINode *pn = builder.CreatePHI(llvm::Type::getInt32Ty(context), 2, "iftmp");

    pn->addIncoming(then_v, thenBB);
    pn->addIncoming(else_v, elseBB);
    VIS_RETURN(pn);
}

void Codegen::Visit(const IdenExpr &expr)
{
    llvm::Value *v = named_values[expr.GetName()];
    if(!v) {
        throw CodegenError(fmt::format("Couldn't find identifier '{}'", expr.GetName()), expr.GetLocation());
    }
    VIS_RETURN(v);
}

void Codegen::Visit(const ReturnExpr &expr)
{
    std::cerr << "ReturnExpr not implemented" << std::endl;
    VIS_RETURN(nullptr);
}

void Codegen::Visit(const CompoundExpr &expr)
{
    llvm::Value *val;
    for(const auto &exp : expr.GetExpressions()) {
        val = VIS_ACCEPT(exp);
    }
    VIS_RETURN(val);
}

void Codegen::Visit(const VarDecl &expr)
{
    std::cerr << "VarDecl not implemented" << std::endl;
    VIS_RETURN(nullptr);
}
