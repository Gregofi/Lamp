# Lamp
Custom Scala inspired functional and object oriented programming language

Example of Lamp program:

```
def Fact(x : Int) : Int = if(x < 2) 1 else x * Fact(x - 1)

def main() : Int = writeln(Fact(5))
```

It is a statically typed language, but so far it only has integers. It will have floats in
the close future. Function as types are also planned and sometime in the future strings will
also be supported.

Lexer and Parser are both hand-written. Lamp compiles this code to the LLVM IR. This can be further compiled with Clang to produce executable code.

It is a toy project for learning the compilers and mainly LLVM IR. 

## How to build
You need CMake, Clang and LLVM.

- Clone this repo
- Create a build folder somewhere
- Run CMake from that folder, as argument give it the lamp folder
- Run makefile
- `lamp` executable is the compiler
- `lamp < Fact.lamp` creates the IR, pass it to Clang like this `clang Fact.ll /path/to/lamp/imports/io.c` 
