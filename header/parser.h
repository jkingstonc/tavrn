//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_PARSER_H
#define TAVRN_PARSER_H

#include "utils.h"
#include "tavrn.h"

struct SymbolTable;
struct SymbolEntry;

typedef enum ParseError{
    PARSE_UNEXPECTED_TOKEN
}ParseError;

typedef enum ASTNodeType{
    PROGRAM,                // The root program

    EXPR_ASSIGN,            // =
    EXPR_LITERAL,           // 3
    EXPR_ARRAY,             // []
    EXPR_VAR,               // x
    EXPR_UNARY,             // not
    EXPR_BIN_CONNECTIVE,    // &, |
    EXPR_LOG_CONNECTIVE,    // and, or
    EXPR_THIS,              // this
    EXPR_CALL,              // ()
    EXPR_STRUCT_GET,        // = a.x
    EXPR_STRUCT_SET,        // a.x =
    EXPR_GROUP,             // ()

    STMT_IMPORT,
    STMT_RETURN,
    STMT_CONTINUE,
    STMT_STRUCT,
    STMT_FUNC,
    STMT_DEFINE,
    STMT_BREAK,
    STMT_FOR_ITER,
    STMT_FOR,
    STMT_WHILE,
    STMT_DO_WHILE,
    STMT_IF,
    STMT_SCOPE,
    STMT_EXPRESSION,


}ASTNodeType;

typedef struct ASTNode{
    ASTNodeType type;
    union {
        struct RootProgramAST{                         // Root program
            List * rootProgramASTNodes;
        }rootProgramAST;
        // Struct
        struct StructAST{
            const char * name;
        }structAST;
        // Expressions
        struct UnaryAST{                         // Unary operator#
            Token u_op;
            struct ASTNode * uOperand;
        }unaryAST;
        struct BinaryAST{                         // Binary operator
            Token b_op;
            struct ASTNode * bLeft;
            struct ASTNode * bRight;
        }binaryAST;

        // Statements
        struct WhileAST{                         // While statement
            struct ASTNode * whileCondition;
            struct ASTNode * whileBody;
        }whileAST;
        struct DoWhileAST{                         // Do while statement
            struct ASTNode * doWhileCondition;
            struct ASTNode * doWhileBody;
        }doWhileAST;
        struct IfAST{                         // If statement
            struct ASTNode * ifCondition;
            struct ASTNode * ifBody;
            List elseIfConditions;
            List elseIfBodies;
            struct ASTNode * elseBody;
        }ifAST;
    };
}ASTNode;

typedef enum VarAttribute{
    STRICT_ASSIGN,          // '!'
}VarAttribute;

typedef enum Directive{
    IS_MAIN,
    IS_INLINE
}Directive;

// Store a map of the macros defined in the directives
typedef struct DirectiveMacroMap{
    Map * symbols;
}DirectiveMacroMap;

// Store some directive-controlled compile time options such as which files to include in compilation
typedef struct CompilerDirectiveInstructions{
    int instructions;                                       // Bitmask for instructions
}CompilerDirectiveInstructions;

typedef struct Parser{
    List * tokens;
    ListNode * currentToken;
    ASTNode * program;
    DirectiveMacroMap * directiveMacroMap;                  // Map of the macros to replace
    CompilerDirectiveInstructions * directiveInstructions;  // The global instructions for the compiler
    ParseError currentError;
}Parser;

Parser * parser;

static Parser * newParser(List * tokens);
void deleteParser();

ASTNode * parse(List * tokens);

static void parseProgram();
static ASTNode * parsePreliminary();
static ASTNode * parseStruct();
static ASTNode * parseEnum();
static ASTNode * parseVarDefine();
static ASTNode * parseFunDefine(uint8_t isStructMember);

static void deleteProgram();
static inline Token * current();
static inline Token * prev();
static inline void parseAdvance(uint32_t amount);
static inline ParseError parseError(ParseError parseError, const char * msg);
static inline int parseConsume(TokenType tokenType, const char * msg);

#endif //TAVRN_PARSER_H
