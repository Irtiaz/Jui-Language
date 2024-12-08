#ifndef IRLEX_H_325346
#define IRLEX_H_325346

#include <stdio.h>
#include "string_stream.h"

#define MAX_TOKEN_CHARS 20
#define BUFFER_SIZE 100

typedef struct IrLexerStruct IrLexer;

IrLexer *createIrLexer(struct string_stream *ruleStream, FILE *inputFile);
void getNextToken(IrLexer *irLexer, char token[MAX_TOKEN_CHARS], char lexeme[BUFFER_SIZE], int *pLineNumber);
void destroyIrLexer(IrLexer *irLexer);

#endif
