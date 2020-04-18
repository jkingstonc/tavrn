//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_TAVRN_H
#define TAVRN_TAVRN_H

#include <ctype.h>
#include <stdint.h>
#include <float.h>

typedef enum TokenType{
    TOK_NULL=0,
    TOK_ASM,
    TOK_NATIVE,         // For c code calls

    TOK_DIRECTIVE,
    TOK_ENUM,
    TOK_STRUCT,
    TOK_IF,
    TOK_ELIF,
    TOK_ELSE,
    TOK_DO,
    TOK_WHILE,
    TOK_FOR,
    TOK_SWITCH,
    TOK_BREAK,
    TOK_CONTINUE,
    TOK_RETURN,
    TOK_VAR,
    TOK_CONST,
    TOK_FUN,
    TOK_THIS,
    TOK_TYPE,

    TOK_FP,
    TOK_BOOL,
    TOK_CHAR,
    TOK_STRING,
    TOK_U8,
    TOK_I8,
    TOK_U16,
    TOK_I16,
    TOK_U32,
    TOK_I32,
    TOK_U64,
    TOK_I64,
    TOK_F32,
    TOK_F64,

    TOK_IDENTIFIER,

    STRING_LITERAL,
    NUMBER_LITERAL,

    TOK_LEFT_CURLY,         // {
    TOK_RIGHT_CURLY,        // }
    TOK_LEFT_BRACET,        // [
    TOK_RIGHT_BRACKET,      // ]
    TOK_LEFT_PAREN,         // (
    TOK_RIGHT_PAREN,        // )
    TOK_COMMA,              // ,
    TOK_PERIOD,             // .
    TOK_SEMICOLON,          // ;
    TOK_COLON,              // :
    TOK_BANG,               // !
    TOK_QUESTION,           // ?

    TOK_PLUS,               // +
    TOK_MINUS,              // -
    TOK_STAR,               // *
    TOK_DIV,                // /
    TOK_PERCENT,            // %
    TOK_ASSIGN,             // =
    TOK_AND_BIN,            // &
    TOK_OR_BIN,             // |
    TOK_XOR_BIN,            // ¬
    TOK_WIGGLE,             // ~
    TOK_LTHAN,              // <
    TOK_GTHAN,              // >
    TOK_INCREMENT,          // ++
    TOK_DECREMENT,          // --
    TOK_LEQ,                // <=
    TOK_GEQ,                // >=
    TOK_LSHIFT,             // <<
    TOK_RSHIFT,             // >>
    TOK_EQUALS,             // ==
    TOK_NOT_EQUALS,         // !=
    TOK_AND_LOGICAL,        // and
    TOK_NOT_LOGICAL,        // not
    TOK_OR_LOGICAL,         // or



}TokenType;

typedef union TokenValue{
    char * string;
}TokenValue;

/**
 * Lexer generates a stream of Tokens from source
 */
typedef struct Token{
    TokenType type;             // Will help us access the union
    int line;                   // The line the token appears on (useful for error messages)
    int indent;                 // The indentation at which the token appears (useful for error messages)
    TokenValue value;           // Alternatively, this could be a void *, however for safety I'm avoiding that
}Token;

// The type of a var
typedef enum Type{
    SYMBOL_TABLE,                   // Used so a symbol table can point to another (used in parser)
    VOID,
    I32
}Type;


#endif //TAVRN_TAVRN_H
