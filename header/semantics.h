//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_SEMANTICS_H
#define TAVRN_SEMANTICS_H

#include "utils.h"
#include "tavrn.h"

typedef struct SymbolEntry{
    char * identifier;                                    // Literal identifier
    Type type;                                            // Type of the symbol
    List attributes;                                      // Optional specifies: Strict assignment, is assured etc
    List directives;                                      // Directives : is main, is inline etc
    struct SymbolTable * parentSymbolTable;               // Used for scope management
}SymbolEntry;

typedef struct SymbolTable{
    int scopeLevel;                             // The level of the scope (0 for global)
    Map * symbols;
}SymbolTable;

typedef struct SymbolTableManager{
    SymbolTable * currentSymbolTable;                       // The current symbol table we are at
    List * allSymbolTables;                                 // Store a pointer to all symbol tables so we can free them
}SymbolTableManager;

typedef struct SemanticAnalyser{
    SymbolTableManager * symbolTableManager;
}SemanticAnalyser;

void analyse();

#endif //TAVRN_SEMANTICS_H
