#include <iostream>
#include <system_error>
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

    std::error_code ec;
    llvm::raw_fd_ostream file("out.ll", ec);
    Codegen cg(file);
    program.Accept(cg);
}
