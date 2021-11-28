#include <iostream>
#include "include/Parser.h"

int main() {
    try{
        Parser(std::cin).ParseBinExpr();
    } catch(ParserError &p) {
        std::cout << p.what() << std::endl;
    }
}
