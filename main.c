#include "header/directives.h"
#include "header/lexer.h"
#include "header/parser.h"
#include "header/optimiser.h"
#include "header/generator.h"
#include "header/debug.h"
#include "header/utils.h"

#include <time.h>


int main() {

    clock_t begin = clock();

    char * source = "struct Test : {   var x : u8; } struct Another : {   }";

    // Perform lexical analysis
    List * tokens = lex(source);

    printf("before directives\n");
    debugTokenList(tokens);

    tokens = processDirectives("currentFile.il", tokens);

    printf("after directives\n");
    debugTokenList(tokens);

    // Parse the tokens generated from the lexer
    // Symbol table stuff happens here
    ASTNode * ast = parse(tokens);
    // Optimise the ast
    ast = optimiseAST(ast);
    // Generate the object file from the ParseTree
    generate(ast);

    deleteLexer();

    deleteDirectivesProcessor();

    deleteParser();

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Compilation time %f seconds\n", time_spent);
    return 0;
}
