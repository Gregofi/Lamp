#include "gtest/gtest.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include <iostream>

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
}