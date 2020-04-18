//
// Created by james on 18/04/2020.
//


#include "header/directives.h"
#include "header/debug.h"

#define END() (directivesProcessor->currentToken == NULL)
#define PEEK() ((Token*)directivesProcessor->currentToken->data)
#define NO_ERROR() (directivesProcessor->currentError==-1)

static DirectivesProcessor * newDirectivesProcessor(List * tokens){
    directivesProcessor = (DirectivesProcessor*)malloc(sizeof(DirectivesProcessor));
    directivesProcessor->tokens = tokens;
    directivesProcessor->usedFiles = (List*)malloc(sizeof(List));
    initList(directivesProcessor->usedFiles);
    directivesProcessor->macroMap = (Map*)malloc(sizeof(Map));
    initMap(directivesProcessor->macroMap, 0, sizeof(char*), sizeof(char *));
    directivesProcessor->currentToken = directivesProcessor->tokens->startNode;
    directivesProcessor->currentTokenIndex = 0;
    directivesProcessor->currentError = -1;
    return directivesProcessor;
}

void deleteDirectivesProcessor(){
    freeList(directivesProcessor->usedFiles);
    freeMap(directivesProcessor->macroMap);
    free(directivesProcessor);
}

List * processDirectives(char * currentFileName, List * tokens){
    printf("starting directive analysis...\n");
    newDirectivesProcessor(tokens);
    // Add the current filename to the list of used filenames to initialise the set of filenames
    addList(directivesProcessor->usedFiles, currentFileName);

    // Loop over every token and process the ones that are macros
    while(!END() && NO_ERROR()){
        if(PEEK()->type==TOK_DIRECTIVE) {
            // printf("directive!\n");
            // Check what the macro actually says
            if (strncmp(PEEK()->value.string, "macro", 5) == 0) {
                char * macroName,  * macroValue;

                dirAdvanceDel(1);  // Consume the #macro token

                if(!dirConsumeDel(TOK_LEFT_BRACET, "Expected '[' after macro!")) break;

                macroName = dirConsumeValueDel("Expected identifier token!");
                if(!macroName) break;
                printf("macro name: %s\n", macroName);

                if(dirConsumeDel(TOK_COMMA, "Expected ',' after macro identifier!")==0) break;

                macroValue = dirConsumeValueDel("Expected value after macro decleration!");
                if(!macroValue) break;
                printf("macro value: %s\n", macroValue);

                if(!dirConsumeDel(TOK_RIGHT_BRACKET, "Expected closing ']' after macro definition!")) break;

                free(macroName);
                free(macroValue);

                /*
                 * Need to figure out how we are going to handle deleting the tokens. the problem is that
                 * if we free the token. The value still exists. So at some point during compilation, we
                 * are gonna have to then free that value.
                 *
                 * An option would be to duplicate the actual value of the value, then free the value.
                 *
                 * The other one which is probably the best is to just free the macro tables which in turn
                 * will free the values.
                 *
                 * */
            }
        }else{
            dirAdvance(1);
        }
    }
    return tokens;
}


static inline void * dirAdvance(uint32_t amount){
    void * data;
    for(int i = 0; i<amount; i++) {
        data = data = ((Token *) directivesProcessor->currentToken->data)->value.string;
        directivesProcessor->currentToken = directivesProcessor->currentToken->next;
    }
    return data;
}

/**
 * Not this probably doesn't work! we need to get the next token before we delete the current token? actually it may work
 * @param amount
 */
static inline void * dirAdvanceDel(uint32_t amount){
    void * data;
    for(int i = 0; i<amount; i++){
        // Retrieve the data before advancing
        data = ((Token*)directivesProcessor->currentToken->data)->value.string;
        ListNode * next = directivesProcessor->currentToken->next;
        // removeList NOT removeTokenList as we want to preserve the data, we will free it later!
        removeList(directivesProcessor->tokens, directivesProcessor->currentTokenIndex);
        directivesProcessor->currentToken = next;
    }
    return data;
}

static inline DirectiveError dirError(DirectiveError dirError, const char * msg){
    printf("-> DIRECTIVES ERROR\n-> %s\n", msg);
    directivesProcessor->currentError=dirError;
    return dirError;
}

static inline uint8_t dirConsumeDel(TokenType tokenType, const char * msg){
    if(END() || PEEK()->type != tokenType){
        dirError(DIR_UNEXPECTED_TOKEN, msg);
        return 0;
    }
    // Then remove the token from the list
    dirAdvanceDel(1);
    return 1;
}

/**
 * Consume a value then delete the token from the token list
 */
static inline void * dirConsumeValueDel(const char * msg){
    if(END() || PEEK()->value.string == NULL){
        dirError(DIR_UNEXPECTED_TOKEN, msg);
        return 0;
    }
    return dirAdvanceDel(1);
//	// Get the value
//	void * value = PEEK()->value.string;
//	// Then delete the actual token
//	removeTokenList(directivesProcessor->tokens, directivesProcessor->currentTokenIndex);
//	// Then return the value
//	return value;
}

static void importFile(char * filename){
//    for(int i = 0; i<directivesProcessor->usedFiles->size; i++){
//        int s1 = strlen(filename);
//        int s2 = strlen((char*)directivesProcessor->usedFiles->array[i]);
//        if(strncmp(filename, (char*)directivesProcessor->usedFiles->array[i], (s1>s2) ? s1 : s2) != 0){
//            // Not already included
//        }
//    }
}
