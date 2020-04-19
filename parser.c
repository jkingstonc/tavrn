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

#define IS_MODIFIER(type) (type==TOK_BANG || type==TOK_QUESTION || type==TOK_STAR)

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
    deleteASTNode(parser->program);
    free(parser);
}

static void deleteASTNode(ASTNode * node){
    switch(node->type){
        case PROGRAM:{
            printf("deleting root program!\n");
            // We have to store the program size otherwise it will change when we delete nodes
            uint32_t programSize = node->rootProgramAST.rootProgramASTNodes->size;
            // Loop over each root node in the program
            for(uint32_t i = 0; i<programSize; i++)
                deleteASTNode(getList(node->rootProgramAST.rootProgramASTNodes, i));
            freeList(node->rootProgramAST.rootProgramASTNodes);
            free(node);
            break;
        }
        case STMT_STRUCT:{
            printf("deleting struct!\n");
            // Loop over each root node in the program
            for(uint32_t i = 0; node->structAST.members->size; i++)
                deleteASTNode(getList(node->structAST.members, i));
            for(uint32_t i = 0; node->structAST.methods->size; i++)
                deleteASTNode(getList(node->structAST.methods, i));
            freeList(node->structAST.members);
            freeList(node->structAST.methods);
            free(node);
            break;
        }
        case STMT_VAR_DEFINE:{
            printf("deleting var!\n");
            if(node->varDefineAST.initialiser)
                deleteASTNode(node->varDefineAST.initialiser);
            free(node);
            break;
        }
    }
}

ASTNode * parse(List * tokens){
    printf("starting parsing...\n");
    parser = newParser(tokens);
    parseProgram();
    return 0;
}


static void parseProgram(){
    printf("parse program\n");

    // Create the root program
    ASTNode * rootProgramAST = (ASTNode*)malloc(sizeof(ASTNode));
    rootProgramAST->type = PROGRAM;
    rootProgramAST->rootProgramAST.rootProgramASTNodes = (List*)malloc(sizeof(List));
    initList(rootProgramAST->rootProgramAST.rootProgramASTNodes);
    // Add the root program to the parser
    parser->program = rootProgramAST;

    // While we haven't reached the end token, add each preliminary AST node to the root program
    while(!END() && NO_ERROR()) addList(rootProgramAST->rootProgramAST.rootProgramASTNodes, parsePreliminary());
}

/**
 * The root items in the program
 */
static ASTNode * parsePreliminary(){
    if(parseConsumePeek(TOK_STRUCT))
        return parseStruct();
    if(parseConsumePeek(TOK_ENUM))
        return parseEnum();
    else
        parseError(PARSE_UNEXPECTED_TOKEN, "Expected struct/enum/functions/variables!");
    return NULL;
}


static ASTNode * parseStruct(){
    printf("--- parsing struct!\n");

    ASTNode * structNode = (ASTNode*)malloc(sizeof(ASTNode));
    structNode->type = STMT_STRUCT;
    structNode->structAST.members = (List*)malloc(sizeof(List));
    structNode->structAST.methods = (List*)malloc(sizeof(List));
    initList(structNode->structAST.members);
    initList(structNode->structAST.methods);

    if(NO_ERROR()) {
        Token * identifier = parseConsume(TOK_IDENTIFIER, "Expected identifier after struct...");
        structNode->structAST.identifier = identifier;
    }
    if(NO_ERROR()) parseConsume(TOK_COLON, "Expected ':' after struct identifier...");
    if(NO_ERROR()) parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");

    /**
     * Need a way to check that we havent reached the end otherwise we will peek nothing :(
     */

    while(!END() && NO_ERROR() && PEEK()->type != TOK_RIGHT_CURLY){
        if(parseConsumePeek(TOK_VAR))
            addList(structNode->structAST.members, parseVarDefine());
        else if(parseConsumePeek(TOK_FUN))
            addList(structNode->structAST.members, parseFunDefine(1));
        else
            parseError(PARSE_UNEXPECTED_TOKEN, "Struct body must either be variable or function!");
    }
    if(NO_ERROR()) parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after struct decleration...");
    return structNode;
}

static ASTNode * parseEnum(){
    printf("--- parsing enum!\n");
    if(NO_ERROR()) parseConsume(TOK_IDENTIFIER, "Expected identifier after enum...");
    if(NO_ERROR()) parseConsume(TOK_COLON, "Expected ':' after enum identifier...");
    if(NO_ERROR()) parseConsume(TOK_LEFT_CURLY, "Expected '{' after ':'...");
    while(!END() && NO_ERROR() && PEEK()->type != TOK_RIGHT_CURLY){

    }
    if(NO_ERROR()) parseConsume(TOK_RIGHT_CURLY, "Expected closing '}' after enum decleration...");
    return 0;
}

static ASTNode * parseVarDefine(){
    ASTNode * varDefine = (ASTNode*)malloc(sizeof(ASTNode));
    varDefine->type = STMT_VAR_DEFINE;
    Token * identifier = parseConsume(TOK_IDENTIFIER, "Expected var identifier!");
    varDefine->varDefineAST.identifier = identifier;
    // Check if we are initialising the variable
    if(parseConsumePeek(TOK_COLON)){
        // Check for any modifiers
        while(IS_MODIFIER(PEEK()->type))
            varDefine->varDefineAST.modifierBits &= getModifierBit(parseAdvance(1)->type);
        // We could be declaring a type, or using a custom structure identifier
        if(IS_TYPE(PEEK()->type) || PEEK()->type==TOK_IDENTIFIER){
            Token * dataType = parseAdvance(1);
            varDefine->varDefineAST.type = dataType->type;
        }
        // Check if we are initiailising the value
        // TODO This isn't very robust as it could mess with the synxax...
        if(PEEK()->type!=TOK_SEMICOLON)
            varDefine->varDefineAST.initialiser = parseExpression();
    }
    parseConsume(TOK_SEMICOLON, "Expected semi-colon after variable decleration!");

    return varDefine;
}

static ASTNode * parseFunDefine(uint8_t isStructMember){
    return 0;
}

static ASTNode * parseExpression(){
    parseAdvance(1);
    return NULL;
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
        case TOK_STAR: return 3;
    }
}
