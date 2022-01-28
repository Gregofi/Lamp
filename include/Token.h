#ifndef TOKEN_H
#define TOKEN_H


/** Location of token in the source */
struct SourceLocation {
    SourceLocation(int row, int column) : row(row), column(column) {}
    int row;
    int column;
};

/**
Contains all possible info about a token.
This class is not intended to be space efficient.
*/
class Token
{
public:
    enum class Kind {
            /* Literals */
            INT_LITERAL,
            FLOAT_LITERAL,
            STRING_LITERAL,
            /* Data types */
            INT,
            DOUBLE,
            STRING,
            /* Other keywords */
            DEF,
            CLASS,
            RETURN,
            /* Operators */
            OP_EQUAL,
            OP_PLUS,
            OP_MINUS,
            OP_ASTERISK,
            OP_DIVIDE,
            OP_ASSIGN,
            OP_GREATER,
            OP_LESS,
            /* Other */
            IDENTIFIER,
            LBRACKET,
            RBRACKET,
            LSQUAREB,
            RSQUAREB,
            LCURLYB,
            RCURLYB,
            IF,
            ELSE,
            DOUBLE_DOT,
            END_OF_INPUT,
            VAL,
            VAR,
            COMMA,
    };

    bool operator==(Token::Kind other) const
    {
        return kind == other;
    }

    bool operator!=(Token::Kind other) const
    {
        return kind != other;
    }

    Token(Kind kind, SourceLocation loc) : kind(kind), loc(loc) {}
    Token(Kind kind, int row, int col) : kind(kind), loc(row, col) {}
    Token() : loc(0, 0) {}
    /** The actual kind of a token */
    Kind kind;

    /** Location in the source file */
    SourceLocation loc;
};

#endif
