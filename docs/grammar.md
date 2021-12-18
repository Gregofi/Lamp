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
Decl     ::= val id : Type | var id : Type
Expr     ::=  
           | if( Expr ) Expr [else Expr]
           | Decl
           | return Expr
           | id BinOp id
           | { [Expr...] }

Function ::= def Id({id : type,}) [: Type] = { Expr }


          










