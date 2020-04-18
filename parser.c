//
// Created by james on 18/04/2020.
//


#include "header/parser.h"
#include <stdio.h>

#define END() (parser->currentToken == NULL)
#define PEEK() ((Token*)parser->currentToken->data)
#define EXPECT(tokenType) (PEEK()->type == tokenType)
#define CONSUME(tokenType) {  \
    if(PEEK() == tokenType)   \
        advance(1);           \
}
#define NO_ERROR() (parser->currentError==-1)

static Parser * newParser(List * tokens){
    parser = (Parser*)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->currentToken = tokens->startNode;
    // Create the root program AST node
    parser->program = (ASTNode*)malloc(sizeof(ASTNode));
    parser->program->type = PROGRAM;
    parser->currentError=-1;
    return parser;
}
void deleteParser(){
    deleteProgram();
    free(parser);
}

ASTNode * parse(List * tokens){
    printf("starting parsing...\n");

    parser = newParser(tokens);

    parseProgram();
    return 0;
}


static void parseProgram(){
    printf("parse program\n");
    // While we haven't reached the end token, add each preliminary AST node to the root programi
    while(!END() && NO_ERROR()){
        parsePreliminary();
        break;
        //insertList(parser->program->rootProgramASTNodes, preliminary(), sizeof(ASTNode*));
    }
}

/**
 * The root items in the program
 */
static ASTNode * parsePreliminary(){
    printf("preliminary!\n");
    switch(PEEK()->type){
        case TOK_STRUCT : {
            return parseStruct();
        }
        case TOK_ENUM : {
            return parseEnum();
        }
    }
    return 0;
}


static ASTNode * parseStruct(){
    parseAdvance(1); // Eat the TOK_STRUCT
    printf("--- parsing struct!\n");
    parseConsume(TOK_IDENTIFIER, "Expected identifier after struct...");
    parseConsume(TOK_COLON, "Expected ':' after struct identifier...");
    parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");
//    while(PEEK()->type != TOK_RIGHT_CURLY){
//        switch(PEEK()->type) {
//            case TOK_VAR: parseVarDefine(); break;
//            case TOK_FUN: parseFunDefine(1); break;
//        }
//    }
    parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after struct decleration...");
    return 0;
}

static ASTNode * parseEnum(){
    parseAdvance(1); // Eat the TOK_ENUM
    printf("--- parsing enum!\n");
    parseConsume(TOK_IDENTIFIER, "Expected identifier after enum...");
    parseConsume(TOK_COLON, "Expected ':' after enum identifier...");
    parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");
    while(PEEK()->type != TOK_RIGHT_CURLY){
    }
    parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after enum decleration...");
    return 0;
}

static ASTNode * parseVarDefine(){
    parseAdvance(1); // Eat the TOK_VAR
    return 0;
}

static ASTNode * parseFunDefine(uint8_t isStructMember){
    return 0;
}

static void deleteProgram(){
    free(parser->program);
}


static inline Token * current(){
    return parser->currentToken->data;
}

static inline void parseAdvance(uint32_t amount){
    for(int i = 0; i<amount; i++)
        parser->currentToken = parser->currentToken->next;
}

static inline ParseError parseError(ParseError parseError, const char * msg){
    printf("-> PARSING ERROR\n-> %s\n", msg);
    parser->currentError=parseError;
    return parseError;
}

static inline int parseConsume(TokenType tokenType, const char * msg){
    if(END() || PEEK()->type != tokenType){
        parseError(PARSE_UNEXPECTED_TOKEN, msg);
        return 0;
    }
    parseAdvance(1);
    return 1;
}