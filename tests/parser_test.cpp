#include "gtest/gtest.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/Nodes/Expr/IfExpr.h"
#include <iostream>
#include <sstream>

TEST(BinaryExprTest, SimpleAE)
{
        std::istringstream iss("1 + 12");
        Parser parser(iss);
        std::unique_ptr<Expr> expr1;
        expr1 = parser.ParseBinExpr();
        BinExpr &expr1b = dynamic_cast<BinExpr&>(*expr1);
        EXPECT_EQ(expr1b.GetOp(), Operator::PLUS);
        int a = std::get<int>(dynamic_cast<LiteralExpr &>(*(expr1b.GetLHS())).GetValue());
        int b = std::get<int>(dynamic_cast<LiteralExpr &>(*(expr1b.GetRHS())).GetValue());
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 12);

        std::istringstream iss2("1 + 13 * 3");
        Parser parser2(iss2);
        std::unique_ptr<Expr> expr2 = parser2.ParseBinExpr();
        BinExpr &expr2b = dynamic_cast<BinExpr&>(*expr2);
        BinExpr &leftsubtree = dynamic_cast<BinExpr&>(*expr2b.GetLHS());
        a = std::get<int>(dynamic_cast<LiteralExpr &>(*(leftsubtree.GetLHS())).GetValue());
        b = std::get<int>(dynamic_cast<LiteralExpr &>(*(leftsubtree.GetRHS())).GetValue());
        int c = std::get<int>(dynamic_cast<LiteralExpr &>(*(expr2b.GetRHS())).GetValue());
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 13);
        EXPECT_EQ(c, 3);
}

TEST(IfExprTest, IfTest)
{
    {
        std::istringstream iss("if(0) 1");
        Parser parser(iss);
        auto expr = parser.ParseIfExpr();
        IfExpr &if_expr = dynamic_cast<IfExpr &>(*expr);
        
        auto cond = dynamic_cast<LiteralExpr &>(*if_expr.GetCond());
        EXPECT_EQ(0, std::get<int>(cond.GetValue()));

        auto body = dynamic_cast<LiteralExpr &>(*if_expr.GetIfBody());
        EXPECT_EQ(1, std::get<int>(body.GetValue()));
    }
    {
        std::istringstream iss("if(1 + 2) 0 else 1");
        Parser parser(iss);
        auto expr = parser.ParseIfExpr();
        IfExpr &if_expr = dynamic_cast<IfExpr &>(*expr);
        auto else_body = dynamic_cast<LiteralExpr &>(*if_expr.GetElseBody());
        EXPECT_EQ(1, std::get<int>(else_body.GetValue()));
    }
}

TEST(ReturnExpr, ReturnTest)
{
    std::istringstream iss("return 0");
    Parser parser(iss);
    auto expr = parser.ParseReturnExpr();
    auto expr1 = dynamic_cast<const LiteralExpr&>(expr->GetExpr()); 
}


TEST(AssignExpr, BasicAssignExpr)
{
    std::istringstream iss("val x : Int");
    Parser parser(iss);
    auto expr = parser.ParseVarDecl();
    VarDecl &decl = dynamic_cast<VarDecl &>(*expr);
    EXPECT_EQ(decl.GetName(), "x");
    EXPECT_EQ(decl.GetType(), Type::INTEGER);
    EXPECT_FALSE(decl.IsMutable());
    EXPECT_FALSE(decl.GetValue()); 
}

TEST(AssignExpr, BasicExprWithVal)
{
    std::istringstream iss("var _x : Double = 1.0");
    Parser parser(iss);
    auto expr = parser.ParseVarDecl();
    VarDecl &decl = dynamic_cast<VarDecl &>(*expr);
    EXPECT_EQ(decl.GetName(), "_x");
    EXPECT_EQ(decl.GetType(), Type::DOUBLE);
    EXPECT_TRUE(decl.IsMutable());
    EXPECT_TRUE(decl.GetValue());
}

TEST(CompoundExpr, BasicCompoundExpr)
{
    std::istringstream iss("{ var x : Int = 1   x + x   return x }");
    Parser parser(iss);
    
    auto expr = parser.ParseCompoundExpr();
    EXPECT_EQ(expr->GetExpressions().size(), 3);
}

TEST(Function, FuncWithoutArgs)
{
    std::istringstream iss("def foo() : Int = 1");
    Parser parser(iss);
    auto func = parser.ParseFunction();
    EXPECT_EQ(func.GetName(), "foo");
    EXPECT_EQ(func.GetReturnType(), Type::INTEGER);
    EXPECT_FALSE(func.GetArguments().size());
}

TEST(Function, FuncWithArgs)
{
    std::istringstream iss("def foo(x : Int, y : Int) : Double = x + y");
    Parser parser(iss);
    auto func = parser.ParseFunction();
    EXPECT_EQ(func.GetName(), "foo");
    EXPECT_TRUE(func.GetArguments().size() == 2);
    EXPECT_EQ(func.GetArguments()[0].name, "x");
    EXPECT_EQ(func.GetArguments()[1].name, "y");
    EXPECT_EQ(func.GetReturnType(), Type::DOUBLE);
}

TEST(Program, ProgramParse)
{
    std::istringstream iss("def main() : Int = 1 + 2\ndef bar(x : Int) : Int = x * 2");
    Parser parser(iss);
    auto program = parser.ParseProgram();
    EXPECT_EQ(program.functions.size(), 2);
}

TEST(FUNCTION, FuncCall)
{
    std::istringstream iss("def foo(x : Int, y : Int) : Int = x + y\n"
                           "def main() : Int = foo(x, x)\n");
    Parser parser(iss); 
    auto program = parser.ParseProgram();
    const auto& func = dynamic_cast<const CallExpr&>(program.functions.at("main").GetBody());
    EXPECT_EQ(func.callee, "foo");
}


