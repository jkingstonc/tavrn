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

#define IS_TYPE(type) (type==TOK_FP || type==TOK_BOOL || type==TOK_CHAR || type==TOK_STRING || type==TOK_U8 \
|| type == TOK_I8 || type == TOK_U16 || type == TOK_I16 || type == TOK_U32 || type == TOK_I32 || type == TOK_U64 \
|| type == TOK_I64 || type == TOK_F32 || type == TOK_F64)

#define IS_MODIFIER(type) (type==TOK_BANG || type==TOK_QUESTION)

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
    if(NO_ERROR()) parseConsume(TOK_IDENTIFIER, "Expected identifier after struct...");
    if(NO_ERROR()) parseConsume(TOK_COLON, "Expected ':' after struct identifier...");
    if(NO_ERROR()) parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");

    /**
     * Need a way to check that we havent reached the end otherwise we will peek nothing :(
     */

    while(NO_ERROR() && PEEK()->type != TOK_RIGHT_CURLY){


        if(parseConsumePeek(TOK_VAR))
            parseVarDefine();
        else if(parseConsumePeek(TOK_FUN))
            parseFunDefine(1);
        else
            parseError(PARSE_UNEXPECTED_TOKEN, "Struct body must either be variable or function!");
    }
    if(NO_ERROR()) parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after struct decleration...");
    return 0;
}

static ASTNode * parseEnum(){
    parseAdvance(1); // Eat the TOK_ENUM
    printf("--- parsing enum!\n");
    if(NO_ERROR()) parseConsume(TOK_IDENTIFIER, "Expected identifier after enum...");
    if(NO_ERROR()) parseConsume(TOK_COLON, "Expected ':' after enum identifier...");
    if(NO_ERROR()) parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");
    while(NO_ERROR() && PEEK()->type != TOK_RIGHT_CURLY){
    }
    if(NO_ERROR()) parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after enum decleration...");
    return 0;
}

static ASTNode * parseVarDefine(){
    ASTNode * varDefine = (ASTNode*)malloc(sizeof(ASTNode));
    Token * identifier = parseConsume(TOK_IDENTIFIER, "Expected var identifier!");
    // Check if we are initialising the variable
    if(parseConsumePeek(TOK_COLON)){
        // Check for any modifiers
        while(IS_MODIFIER(PEEK()->type))
            varDefine->varAST.modifierBits &= getModifierBit(parseAdvance(1)->type);
        // We could be declaring a type, or using a custom structure identifier
        if(IS_TYPE(PEEK()->type) || PEEK()->type==TOK_IDENTIFIER){
            Token * dataType = parseAdvance(1);
            varDefine->varAST.type = dataType->type;
        }
        // Check if we are initiailising the value
        // TODO This isn't very robust as it could mess with the synxax...
        if(PEEK()->type!=TOK_SEMICOLON)
            varDefine->varAST.initialiser = parseExpression();
    }

    free(varDefine);
    printf("var: %s\n", identifier->value.data);
    parseConsume(TOK_SEMICOLON, "Expected semi-colon after variable decleration!");
    return 0;
}

static ASTNode * parseFunDefine(uint8_t isStructMember){
    return 0;
}

static ASTNode * parseExpression(){
    parseAdvance(1);
    return NULL;
}

static void deleteProgram(){
    free(parser->program);
}

static inline Token * parseAdvance(uint32_t amount){
    void * token;
    for(int i = 0; i<amount; i++) {
        token = (Token*)parser->currentToken->data;
        parser->currentToken = parser->currentToken->next;
    }
    return token;
}

static inline ParseError parseError(ParseError parseError, const char * msg){
    printf("-> PARSING ERROR\n-> %s\n", msg);
    parser->currentError=parseError;
    return parseError;
}

/*
* Only advance if what we are peeking is what we are expecting
*/
static inline Token * parseConsumePeek(TokenType tokenType){
    if(PEEK()->type == tokenType)
        return parseAdvance(1);
    return NULL;
}

static inline Token * parseConsume(TokenType tokenType, const char * msg){
    if(END() || PEEK()->type != tokenType){
        parseError(PARSE_UNEXPECTED_TOKEN, msg);
        return 0;
    }
    return parseAdvance(1);
}

static inline uint8_t getModifierBit(TokenType tokenType){
    switch(tokenType){
        case TOK_BANG: return 1;
        case TOK_QUESTION: return 2;
    }
}
