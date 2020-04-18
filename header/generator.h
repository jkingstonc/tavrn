//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_GENERATOR_H
#define TAVRN_GENERATOR_H

#define PLATFORM_WINDOWS
#define ARCHITECTURE_X64

#include "parser.h"

#define GENERATOR_SUCCESS 0
#define GENERATOR_FAIL 1

int generate(ASTNode * ast);

#endif //TAVRN_GENERATOR_H
