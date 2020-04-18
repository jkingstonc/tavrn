//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_DIRECTIVES_H
#define TAVRN_DIRECTIVES_H


#include "utils.h"
#include "tavrn.h"

#include <stdlib.h>
#include <string.h>

typedef enum DirectiveError{
    DIR_UNEXPECTED_TOKEN
}DirectiveError;

typedef struct DirectivesProcessor{
    List * usedFiles;
    List * tokens;
    ListNode * currentToken;
    uint8_t currentTokenIndex;
    DirectiveError currentError;
    Map * macroMap;
}DirectivesProcessor;

DirectivesProcessor * directivesProcessor;

static DirectivesProcessor * newDirectivesProcessor(List * tokens);
void deleteDirectivesProcessor();
List * processDirectives(char * currentFileName, List * tokens);


static inline void * dirAdvance(uint32_t amount);
static inline void * dirAdvanceDel(uint32_t amount);
static inline DirectiveError dirError(DirectiveError dirError, const char * msg);
static inline uint8_t dirConsumeDel(TokenType tokenType, const char * msg);
static inline void * dirConsumeValueDel(const char * msg);


#endif //TAVRN_DIRECTIVES_H
