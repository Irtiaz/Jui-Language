#ifndef IR_PARSER_543547
#define IR_PARSER_543547

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "IrLexer.h"
#include "string_stream.h"

#define BUFFER_SIZE 100
#define MAX_TOKEN_CHARS 20

typedef struct {
	char *key;
	int value;
} IrParseStringHashMapItem;

typedef struct {
	int variableColumnIndex;
	int popCount;
} IrParseRuleDescriptor;


typedef struct {
	IrType value;
	char lexVal[BUFFER_SIZE];
} IrParseStackItem;

typedef struct {
	char token[MAX_TOKEN_CHARS];
	char lexeme[BUFFER_SIZE];
	int lineNumber;
} TokenLexemePair;

typedef struct {
	IrParseStringHashMapItem *tokenMap;
	IrParseRuleDescriptor *descriptors;
	int **table;
	int rowCount;
	int columnCount;

	IrType (**ruleHandlers)(IrParseStackItem *items);
	void (*errorHandler)(TokenLexemePair *erroneousToken);

	IrLexer *irLexer;
} IrParser;

TokenLexemePair IrParseGetNextPair(IrLexer *irLexer);

IrParser *createIrParser(struct string_stream *rulepackStream, FILE *inputFile, void (*errorHandler)(TokenLexemePair *erroneousToken), ...);
IrType irParse(IrParser *irParser);
void destroyIrParser(IrParser *irParser);

IrParser *createIrParser(struct string_stream *rulepackStream, FILE *inputFile, void (*errorHandler)(TokenLexemePair *erroneousToken), ...) {
	IrParser *irParser;

	IrParseStringHashMapItem *tokenMap;
	int rowCount, columnCount;
	IrParseRuleDescriptor *descriptors;
	int **table;
	IrType (**ruleHandlers)(IrParseStackItem*);
	IrLexer *irLexer;


	sh_new_strdup(tokenMap);
	{
		int numberOfTokens, tokenStart;
		string_stream_read_int(rulepackStream, &numberOfTokens);
		string_stream_read_int(rulepackStream, &tokenStart);

		{
			int i;
			for (i = 0; i < numberOfTokens; ++i) {
				char tokenName[MAX_TOKEN_CHARS];
				string_stream_read_string(rulepackStream, tokenName);

				shput(tokenMap, tokenName, tokenStart++);
			}
		}

		shput(tokenMap, "#", tokenStart++);
		shput(tokenMap, "$", tokenStart++);

		columnCount = tokenStart;
	}

	{
		int ruleDescriptorCount;
		string_stream_read_int(rulepackStream, &ruleDescriptorCount);

		descriptors = (IrParseRuleDescriptor *)malloc(sizeof(IrParseRuleDescriptor) * ruleDescriptorCount);

		{
			int i;
			for (i = 0; i < ruleDescriptorCount; ++i) {
				string_stream_read_int(rulepackStream, &descriptors[i].variableColumnIndex);
				string_stream_read_int(rulepackStream, &descriptors[i].popCount);
			}
		}

		{
			va_list rulesArgumentList;
			int i;

			ruleHandlers = (IrType(**)(IrParseStackItem*))malloc(sizeof(IrType(**)(IrParseStackItem*)) * ruleDescriptorCount);

			va_start(rulesArgumentList, ruleDescriptorCount);

			for (i = 0; i < ruleDescriptorCount; ++i) {
				ruleHandlers[i] = va_arg(rulesArgumentList, IrType(*)(IrParseStackItem*));
			}

			va_end(rulesArgumentList);
		}
	}

	{
		string_stream_read_int(rulepackStream, &rowCount);

		table = (int **)malloc(rowCount * sizeof(int *));
		{
			int row;
			for (row = 0; row < rowCount; ++row) {
				table[row] = (int *)malloc(columnCount * sizeof(int));
			}
		}

		{
			int row;
			for (row = 0; row < rowCount; ++row) {
				int column;
				for (column = 0; column < columnCount; ++column) {
					string_stream_read_int(rulepackStream, &table[row][column]);
				}
			}
		}
	}

	irLexer = createIrLexer(rulepackStream, inputFile);

	irParser = (IrParser *)malloc(sizeof(IrParser));
	irParser->tokenMap = tokenMap;
	irParser->descriptors = descriptors;
	irParser->table = table;
	irParser->rowCount = rowCount;
	irParser->columnCount = columnCount;
	irParser->ruleHandlers = ruleHandlers;
	irParser->errorHandler = errorHandler;
	irParser->irLexer = irLexer;

	return irParser;
}

IrType irParse(IrParser *irParser) {
	IrParseStringHashMapItem *tokenMap = irParser->tokenMap;
	IrParseRuleDescriptor *descriptors = irParser->descriptors;
	int **table = irParser->table;
	IrType(**ruleHandlers)(IrParseStackItem*) = irParser->ruleHandlers;

	int *stateStack = NULL;
	TokenLexemePair nextPair = IrParseGetNextPair(irParser->irLexer);
	int lookahead = shget(tokenMap, nextPair.token);
	int inputRightEndMarker = shget(tokenMap, "$");

	IrParseStackItem *dataStack = NULL;
	IrType result;

	int error = 0;

	arrput(stateStack, 0);

	while (1) {
		int currentState = stateStack[arrlen(stateStack) - 1]; 
		int tableValue = table[currentState][lookahead];

		if (currentState == 1 && lookahead == inputRightEndMarker) {
			break;
		}

		if (tableValue) {

			if (tableValue > 0) {
				arrput(stateStack, tableValue);
				{
					IrParseStackItem item = {0};
					strcpy(item.lexVal, nextPair.lexeme);
					arrput(dataStack, item);
				}

				nextPair = IrParseGetNextPair(irParser->irLexer);
				lookahead = shget(tokenMap, nextPair.token);
			}
			else {
				int ruleIndex = -tableValue - 1;
				IrParseRuleDescriptor descriptor = descriptors[ruleIndex];
				int i;

				IrParseStackItem *params = (IrParseStackItem *)malloc(sizeof(IrParseStackItem) * descriptor.popCount);
				for (i = arrlen(dataStack) - descriptor.popCount; i < arrlen(dataStack); ++i) {
					params[i - arrlen(dataStack) + descriptor.popCount] = dataStack[i];
				}

				for (i = 0; i < descriptor.popCount; ++i) {
					arrdel(stateStack, arrlen(stateStack) - 1);
					if (ruleHandlers[ruleIndex]) arrdel(dataStack, arrlen(dataStack) - 1);
				}

				if (ruleHandlers[ruleIndex]) {
					IrType value = (*ruleHandlers[ruleIndex])(params);
					IrParseStackItem item = {0};
					item.value = value;
					arrput(dataStack, item);
				}
				free(params);

				currentState = stateStack[arrlen(stateStack) - 1]; 
				tableValue = table[currentState][descriptor.variableColumnIndex];

				if (tableValue) arrput(stateStack, tableValue);
				else {
					error = 1;
					break;
				}
			}

		}

		else {
			error = 1;
			break;
		}
	}

	result = dataStack[arrlen(dataStack) - 1].value;

	arrfree(stateStack);
	arrfree(dataStack);

	if (error) {
		TokenLexemePair *erroneousToken = (TokenLexemePair *)malloc(sizeof(TokenLexemePair));
		memcpy(erroneousToken, &nextPair, sizeof(TokenLexemePair));
		(*irParser->errorHandler)(erroneousToken);
	}

	return result;
}

void destroyIrParser(IrParser *irParser) {
	int row;
	for (row = 0; row < irParser->rowCount; ++row) {
		free(irParser->table[row]);
	}
	free(irParser->table);
	free(irParser->descriptors);
	shfree(irParser->tokenMap);
	free(irParser->ruleHandlers);
	destroyIrLexer(irParser->irLexer);

	free(irParser);
}


TokenLexemePair IrParseGetNextPair(IrLexer *irLexer) {
	char token[MAX_TOKEN_CHARS];
	char lexeme[BUFFER_SIZE];
	TokenLexemePair pair;

	getNextToken(irLexer, token, lexeme, &pair.lineNumber);
	/*printf("%s %s %d\n", token, lexeme, pair.lineNumber);*/

	strcpy(pair.token, token); 
	strcpy(pair.lexeme, lexeme);

	return pair;
}

#endif
