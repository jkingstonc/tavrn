//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_LEXER_H
#define TAVRN_LEXER_H


#include "utils.h"
#include "tavrn.h"

#include <stdlib.h>
#include <string.h>

typedef enum LexError{
    INVALID_NUMERIC,
    EXPECTED_DELIM,
    UNEXPECTED_CHAR,
    UNEXPECTED_EOF,
    REALLOCATION_FAILIURE
}LexError;

typedef struct Lexer{
    List * tokens;
    char * source;
    char * currentChar;
    int sourceLength;
    int charsConsumed;
    int savePointIndent;        // Used to calculate the correct indent for tokens
    int currentIndent;
    int currentLine;
    LexError currentError;
}Lexer;

Lexer * lexer;

static Lexer * newLexer(char * input);
void deleteLexer();

List * lex(char * source);

static inline void addTokenValue(TokenType tokenType, TokenValue value);
static inline void addToken(TokenType tokenType);
static inline void resetSavePoint();
static inline void newline();
static inline char getNext();
static inline void advance(int amount);
static inline LexError error(const char * msg, LexError lexError);
static void commentBlock();
static int isNumberCharacter(const char c);
static void stringLiteral(const char quoteType);
static void alpha(const char startingChar);
static void numeric(const char startingDigit, int base);
static void directive();
static void skipWhitespace();
static int checkKeyword(const char * nextChars, TokenType keywordType);



#endif //TAVRN_LEXER_H
