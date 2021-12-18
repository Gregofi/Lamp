## Context-free grammar:
literal  ::= ['-'] integerLiteral
           | ['-'] floatingLiteral
           | booleanLiteral
           | characterLiteral
           | stringLiteral
           | symbolLiteral
 
BinOp    ::= + | * | / | - | == | && | ||    
QualId   ::= id
Type     ::= type
Expr     ::=  
           | if( Expr ) Expr [else Expr]
           | return Expr
           | id BinOp id
           | { [Expr...] }

Function ::= def Id({id : type,}) = { Expr }


          










