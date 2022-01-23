#include "include/Codegen.h"
#include "include/Nodes/Program.h"

void Codegen::Visit(const Program &program)
{
    for(const auto &f : program.functions) {
        f.second.Accept(*this);
    }
    module->print(llvm::errs(), nullptr);
}
