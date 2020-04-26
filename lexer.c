//
// Created by james on 18/04/2020.
//


#include "header/lexer.h"

#include <stdio.h>
// Non-header functions


#define END() (lexer->currentChar[0]==EOF || lexer->currentChar[0]=='\0' || lexer->charsConsumed > lexer->sourceLength) ? 1 : 0

#define PEEK() (lexer->currentChar[0])
#define PEEK_MORE() (lexer->currentChar[1])
#define PEEK_AMOUNT(amount) (lexer->currentChar[amount])

#define EXPECT(c) ((c==PEEK()) ? 1 : 0)

#define EXPECT2(c, v) ((c==PEEK() || v==PEEK()) ? 1 : 0)

// Expect a sequence of characters
#define EXPECT_SEQ(c, v) ((PEEK()==c && PEEK_MORE()==v) ? 1 : 0)

#define NO_ERROR() (lexer->currentError==-1)

#define CONSUME(c, errorMsg, errorType) (if(c!=PEEK()) error(errorMsg, errorType));

#define IS_IDENTIFIER_CHAR(c) (isalpha(c) || isNumberCharacter(c) || c=='_')

// Header functions
static Lexer * newLexer(char * source){
    lexer = (Lexer*) malloc(sizeof(Lexer));
    lexer->tokens = (List*)malloc(sizeof(List));
    initList(lexer->tokens);
    lexer->source = source;
    lexer->currentChar = lexer->source;
    lexer->sourceLength = strlen(source);
    lexer->charsConsumed=0;
    lexer->savePointIndent=1;
    lexer->currentIndent=1;
    lexer->currentLine=1;
    lexer->currentError=-1;
    return lexer;
}

void deleteLexer(){
    //freeList(lexer->tokens);
    freeTokenList(lexer->tokens);
    free(lexer);
}

List * lex(char * source){
    printf("starting lexical analysis...\n");
    newLexer(source);
    while(!END() && NO_ERROR()){
        skipWhitespace();
        const char c = getNext();
        switch(c){
            case '/': {
                if(EXPECT('/') || EXPECT('*'))
                    commentBlock();
                else
                    addToken(TOK_DIV);
                break;
            }
            case '{': addToken(TOK_LEFT_CURLY); break;
            case '}': addToken(TOK_RIGHT_CURLY); break;
            case '[': addToken(TOK_LEFT_BRACET); break;
            case ']': addToken(TOK_RIGHT_BRACKET); break;
            case '(': addToken(TOK_LEFT_PAREN); break;
            case ')': addToken(TOK_RIGHT_PAREN); break;
            case ',': addToken(TOK_COMMA); break;
            case '.': addToken(TOK_PERIOD); break;
            case ';': addToken(TOK_SEMICOLON); break;
            case ':': addToken(TOK_COLON); break;
            case '?': addToken(TOK_QUESTION); break;
            case '*': addToken(TOK_STAR); break;
            case '%': addToken(TOK_PERCENT); break;
            case '&': addToken(TOK_AND_BIN); break;
            case '|': addToken(TOK_OR_BIN); break;
            case '~': addToken(TOK_WIGGLE); break;
            case '+': {
                if(PEEK()=='+'){addToken(TOK_INCREMENT); advance(1);}
                else addToken(TOK_PLUS);
                break;
            }
            case '-': {
                if(PEEK()=='-'){addToken(TOK_DECREMENT); advance(1);}
                else addToken(TOK_MINUS);
                break;
            }
            case '<': {
                if(PEEK()=='<') {addToken(TOK_LSHIFT); advance(1);}
                else if(PEEK()=='='){addToken(TOK_LEQ); advance(1);}
                else addToken(TOK_LTHAN);
                break;
            }
            case '>': {
                if(PEEK()=='>') {addToken(TOK_RSHIFT); advance(1);}
                else if(PEEK()=='='){addToken(TOK_GEQ); advance(1);}
                else addToken(TOK_GTHAN);
                break;
            }
            case '!': {
                if(PEEK()=='='){addToken(TOK_NOT_EQUALS); advance(1);}
                else addToken(TOK_BANG);
                break;
            }
            case '=' : {
                if(PEEK()=='='){addToken(TOK_EQUALS); advance(1);}
                else addToken(TOK_ASSIGN);
                break;
            }
            case '\'':
            case '\"':stringLiteral(c); break;

            case '#': directive(); break;

            default:
            {
                // If the token is not a key-symbol or a keyword, it must be an identifier or a number literal
                // First check if it a number, numbers can start with alpha prefixes
                // e.g. h_64,
                if(isdigit(c)) // We know we definitely have a number
                    numeric(c,10);  // Assume base 10
                else if(isalpha(c))
                    alpha(c);
                else
                    error("Unexpected character!", UNEXPECTED_CHAR);
		break;
            }
        }
        if(!NO_ERROR()) break;
    }
    printf("done lexing!\n");
    return lexer->tokens;
}

static inline void addTokenValue(TokenType tokenType, TokenValue value){
    Token * token = (Token*) malloc(sizeof(Token));
    token->type=tokenType;
    token->value=value;
    token->line = lexer->currentLine;
    token->indent = lexer->savePointIndent;
    addList(lexer->tokens, token);
    resetSavePoint();
}

static inline void addToken(TokenType tokenType){
    Token * token = (Token*) malloc(sizeof(Token));
    token->type=tokenType;
    token->value.data = NULL;
    token->line = lexer->currentLine;
    token->indent = lexer->savePointIndent;
    addList(lexer->tokens, token);
    resetSavePoint();
}

static inline void resetSavePoint(){
    lexer->savePointIndent=lexer->currentIndent;
}

static inline void newline(){
    lexer->currentLine++;
    lexer->currentIndent = 0;
}

static inline char getNext(){
    const char c = lexer->currentChar[0];
    advance(1);
    return c;
}

static inline void advance(int amount){
    lexer->currentChar+=amount;
    lexer->charsConsumed+=amount;
    lexer->currentIndent+=amount;
}

static inline LexError error(const char * msg, LexError lexError){
    printf("-> LEXING ERROR\n-> LINE %d CHAR %d\n-> %s\n", lexer->currentLine, lexer->currentIndent, msg);
    lexer->currentError=lexError;
    return lexError;
}

// Needs optimising as we already know its a comment block
static void commentBlock(){
    if(EXPECT('/')){
        while(!EXPECT2('\n', '\r'))
            advance(1);
        newline();
    }
    else if(EXPECT('*')){
        while(!EXPECT_SEQ('*', '/') && NO_ERROR()) {
            if(PEEK()=='\n' || PEEK()=='\r') newline();
            // Need to check for EOF
            advance(1);
            if(END()) error("comment block not delimited!", EXPECTED_DELIM);
        }
        advance(2);
    }
}

static inline int isNumberCharacter(const char c){
    return (isdigit(c) || c=='_' || c=='.');
}

static inline void appendString(char * string, const char c){
    size_t len = strlen(string);
    string[len] = c;
    string[len + 1] = '\0';
}

static void stringLiteral(const char quoteType){
    char * str = malloc(sizeof(char));
    *str ='\0';
    while(!EXPECT(quoteType)){
        char c = getNext();
        str = (char*)realloc(str, strlen(str) * sizeof(char) + 2);
        if(str==NULL) {
            error("memory reallocation failed!", REALLOCATION_FAILIURE);
            return;
        }
        appendString(str, c);
    }
    TokenValue tokenValue = {.data = str};
    addTokenValue(STRING_LITERAL, tokenValue);
    advance(1);
}

static void alpha(const char startingChar){
    switch(startingChar){
        case 'b' : {
            if(checkKeyword("ool", TOK_BOOL)) return;
            else if(checkKeyword("reak", TOK_BREAK)) return;
        }
        case 'c' : {
            if(checkKeyword("har", TOK_CHAR)) return;
            else if(checkKeyword("onst", TOK_CONST)) return;
            else if(checkKeyword("ontinue", TOK_CONTINUE)) return;
        }
        case 'd' : checkKeyword("o", TOK_DO); return;
        case 'e' : {
            if(checkKeyword("num", TOK_ENUM)) return;
            else if(checkKeyword("lif", TOK_ELIF)) return;
            else if(checkKeyword("lse", TOK_ELSE)) return;
        }
        case 'f' : {
            if(checkKeyword("p", TOK_FP)) return;
            else if(checkKeyword("or", TOK_FOR)) return;
            else if(checkKeyword("un", TOK_FUN)) return;
            else if(checkKeyword("32", TOK_F32)) return;
            else if(checkKeyword("64", TOK_F64)) return;
        }
        case 'i' : {
            if(checkKeyword("f", TOK_IF)) return;
            else if(checkKeyword("8", TOK_I8)) return;
            else if(checkKeyword("16", TOK_I16)) return;
            else if(checkKeyword("32", TOK_I32)) return;
            else if(checkKeyword("64", TOK_I64)) return;
        }
        case 's' : {
            if(checkKeyword("truct", TOK_STRUCT)) return;
            else if(checkKeyword("tring", TOK_STRING)) return;
            else if(checkKeyword("witch", TOK_SWITCH)) return;
        }
        case 'r' : checkKeyword("eturn", TOK_RETURN); return;
        case 't' : {
            if(checkKeyword("his",TOK_THIS)) return;
            else if(checkKeyword("type",TOK_TYPE)) return;
        }
        case 'u' : {
            if(checkKeyword("8", TOK_U8)) return;
            else if(checkKeyword("16", TOK_U16)) return;
            else if(checkKeyword("32", TOK_U32)) return;
            else if(checkKeyword("64", TOK_U64)) return;
        }
        case 'v' : if(checkKeyword("ar", TOK_VAR)) return;
        case 'w' : if(checkKeyword("hile", TOK_WHILE)) return;
    }



    char * identifier = malloc(sizeof(char)*2);
    identifier[0] =startingChar;
    identifier[1] = '\0';
    while(IS_IDENTIFIER_CHAR(PEEK())){
        const char c = getNext();
        identifier = realloc(identifier, strlen(identifier) * sizeof(char) + 2);
        if(identifier==NULL) {
            error("memory reallocation failed!", REALLOCATION_FAILIURE);
            return;
        }
        appendString(identifier, c);
    }
    TokenValue value = {.data = identifier};
    addTokenValue(TOK_IDENTIFIER, value);
}

static void numeric(const char startingDigit, int base){
    int containsPeriod = 0;
    char * number = malloc(sizeof(char)*2);
    number[0] = startingDigit;
    number[1] = '\0';
    while(isNumberCharacter(PEEK()) && NO_ERROR()){
        char c = getNext();
        if(c=='.') {
            if(!containsPeriod)
                containsPeriod=1;
            else
                error("Float cannot have multiple decimal points!", INVALID_NUMERIC);
        }
        if(c!='_'){
            number = realloc(number, strlen(number) * sizeof(char) + 2);
            if(number==NULL) {
                error("memory reallocation failed!", REALLOCATION_FAILIURE);
                return;
            }
            appendString(number, c);
        }
    }
    TokenValue tokenValue = {.data = number};
    addTokenValue(NUMBER_LITERAL, tokenValue);
}

static void directive(){
    skipWhitespace();
    char * directive = malloc(sizeof(char)*1);
    directive[0] = '\0';
    while(IS_IDENTIFIER_CHAR(PEEK())){
        const char c = getNext();
        directive = realloc(directive, strlen(directive) * sizeof(char) + 2);
        if(directive==NULL) {
            error("memory reallocation failed!", REALLOCATION_FAILIURE);
            return;
        }
        appendString(directive, c);
    }
    TokenValue value = {.data = directive};
    addTokenValue(TOK_DIRECTIVE, value);
}

static void skipWhitespace(){
    char next = PEEK();
    while(1){
        if(next == '\n' || next == '\r')
            newline();
        else if(next != ' ' && next != '\t')
            break;
        advance(1);
        next = PEEK();
    }
    resetSavePoint();
}

static int checkKeyword(const char * nextChars, TokenType keywordType){
    int len = strlen(nextChars);
    // Check if the nextChars form part of an individual word, and are not exceeding the total char length
    if(lexer->charsConsumed + len > lexer->sourceLength)
        return 0;
    else if(isalpha(PEEK_AMOUNT(len)))
        return 0;

    char nextChar = PEEK();
    for(int i = 0; i<len; i++){
        if(nextChar!=nextChars[i])
            return 0;
        nextChar=PEEK_AMOUNT(i+1);
    }
    addToken(keywordType);
    advance(len);
    return 1;
}
