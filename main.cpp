#include <iostream>
#include "include/Parser.h"
#include "include/ASTPrinter.h"
#include "include/Codegen.h"

int main() {
    Program program;
    Parser parser(std::cin);
    try{
        program = parser.ParseProgram();
    } catch(ParserError &p) {
        std::cout << p.what() << std::endl;
    }
    ASTPrinter ap;
    // program.Accept(ap);

    Codegen cg;
    program.Accept(cg);
}
