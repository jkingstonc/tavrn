//
// Created by james on 07/04/2020.
//

#include "header/debug.h"

const char * typeDebugStrings[] = {
        "TOK_NULL",
        "TOK_ASM",
        "TOK_NATIVE",

        "TOK_DIRECTIVE",
        "TOK_ENUM",
        "TOK_STRUCT",
        "TOK_IF",
        "TOK_ELIF",
        "TOK_ELSE",
        "TOK_DO",
        "TOK_WHILE",
        "TOK_FOR",
        "TOK_SWITCH",
        "TOK_BREAK",
        "TOK_CONTINUE",
        "TOK_RETURN",
        "TOK_VAR",
        "TOK_CONST",
        "TOK_FUN",
        "TOK_THIS",
        "TOK_TYPE",

        "TOK_FP",
        "TOK_BOOL",
        "TOK_CHAR",
        "TOK_STRING",
        "TOK_U8",
        "TOK_I8",
        "TOK_U16",
        "TOK_I16",
        "TOK_U32",
        "TOK_I32",
        "TOK_U64",
        "TOK_I64",
        "TOK_F32",
        "TOK_F64",

        // Identifiers
        "TOK_IDENTIFIER",
        // Constants

        // Literals
        "STRING_LITERAL",
        "NUMBER_LITERAL",

        // Symbols
        "TOK_LEFT_CURLY",
        "TOK_RIGHT_CURLY",
        "TOK_LEFT_BRACET",
        "TOK_RIGHT_BRACKET",
        "TOK_LEFT_PAREN",
        "TOK_RIGHT_PAREN",
        "TOK_COMMA",
        "TOK_PERIOD",
        "TOK_SEMICOLON",
        "TOK_COLON",
        "TOK_BANG",
        "TOK_QUESTION",

        "TOK_PLUS",               // +
        "TOK_MINUS",              // -
        "TOK_STAR",               // *
        "TOK_DIV",                // /
        "TOK_PERCENT",            // %
        "TOK_ASSIGN",             // =
        "TOK_AND_BIN",            // &
        "TOK_OR_BIN",             // |
        "TOK_XOR_BIN",            // ¬
        "TOK_WIGGLE",             // ~
        "TOK_LTHAN",              // <
        "TOK_GTHAN",              // >
        "TOK_INCREMENT",          // ++
        "TOK_DECREMENT",          // --
        "TOK_LEQ",                // <=
        "TOK_GEQ",                // >=
        "TOK_LSHIFT",             // <<
        "TOK_RSHIFT",             // >>
        "TOK_EQUALS",             // ==
        "TOK_NOT_EQUALS",         // !=
        "TOK_AND_LOGICAL",        // and
        "TOK_NOT_LOGICAL",        // not
        "TOK_OR_LOGICAL",         // or
};

void debugToken(Token * token){
    printf("%s\n", typeDebugStrings[token->type]);
}

void debugTokenList(List * list){
    ListNode * current = list->startNode;
    while (current != NULL) {
        Token * token = current->data;
        switch(token->type){
            case TOK_ASM:
            case TOK_DIRECTIVE:
            case TOK_IDENTIFIER:
            case STRING_LITERAL:
                printf("Token: %s, l: %d i: %d, value: %s\n", typeDebugStrings[token->type], token->line, token->indent, (char*)(token->value.string)); break;
            default:
                printf("Token: %s, l: %d i: %d\n", typeDebugStrings[token->type], token->line, token->indent); break;
        }

        current = current->next;
    }
}