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
    
}
