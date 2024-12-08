#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include <stdio.h>
#include <string.h>

#include "string_stream.h"

#define defaultName "__a__"

typedef enum {
	e_start_statements,
	e_statements_statements_statement,
	e_statements_statement,
	e_statement_variable_ASSIGNOP_logic_expression,
	e_statement_PORO_variable,
	e_statement_LIKHO_logic_expression,
	e_statement_loop,
	e_statement_conditional,
	e_logic_expression_rel_expression,
	e_logic_expression_rel_expression_LOGICOP_rel_expression,
	e_logic_expression_logic_expression_NA,
	e_rel_expression_simple_expression,
	e_rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP,
	e_rel_expression_rel_expression_NA,
	e_simple_expression_term,
	e_simple_expression_simple_expression_ADDOP_term,
	e_term_unary_expression,
	e_term_term_MULOP_unary_expression,
	e_unary_expression_ADDOP_unary_expression,
	e_unary_expression_factor,
	e_factor_variable,
	e_factor_NUMBER,
	e_factor_LPAREN_logic_expression_RPAREN,
	e_variable_ID,
	e_variable_ID_LTHIRD_logic_expression_RTHIRD,
	e_loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO,
	e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause,
	e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause,
	e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses,
	e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO,
	e_else_if_clauses_else_if_clauses_else_if_clause,
	e_else_if_clauses_else_if_clause,
	e_else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO,
	e_else_clause_AR_NOILE_statements_KORO
} Rule;

typedef struct {
	Rule rule;
	char lexVal[100];
} Node;

#define ParseTreeContentType Node
#include "ParseTree.h"

#define IrType ParseTree *
#include "IrParser.h"


ParseTree *start_statements(IrParseStackItem *items);
ParseTree *statements_statements_statement(IrParseStackItem *items);
ParseTree *statements_statement(IrParseStackItem *items);
ParseTree *statement_variable_ASSIGNOP_logic_expression(IrParseStackItem *items);
ParseTree *statement_PORO_variable(IrParseStackItem *items);
ParseTree *statement_LIKHO_logic_expression(IrParseStackItem *items);
ParseTree *statement_loop(IrParseStackItem *items);
ParseTree *statement_conditional(IrParseStackItem *items);
ParseTree *logic_expression_rel_expression(IrParseStackItem *items);
ParseTree *logic_expression_rel_expression_LOGICOP_rel_expression(IrParseStackItem *items);
ParseTree *logic_expression_logic_expression_NA(IrParseStackItem *items);
ParseTree *rel_expression_simple_expression(IrParseStackItem *items);
ParseTree *rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP(IrParseStackItem *items);
ParseTree *rel_expression_rel_expression_NA(IrParseStackItem *items);
ParseTree *simple_expression_term(IrParseStackItem *items);
ParseTree *simple_expression_simple_expression_ADDOP_term(IrParseStackItem *items);
ParseTree *term_unary_expression(IrParseStackItem *items);
ParseTree *term_term_MULOP_unary_expression(IrParseStackItem *items);
ParseTree *unary_expression_ADDOP_unary_expression(IrParseStackItem *items);
ParseTree *unary_expression_factor(IrParseStackItem *items);
ParseTree *factor_variable(IrParseStackItem *items);
ParseTree *factor_NUMBER(IrParseStackItem *items);
ParseTree *factor_LPAREN_logic_expression_RPAREN(IrParseStackItem *items);
ParseTree *variable_ID(IrParseStackItem *items);
ParseTree *variable_ID_LTHIRD_logic_expression_RTHIRD(IrParseStackItem *items);
ParseTree *loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO(IrParseStackItem *items);
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause(IrParseStackItem *items);
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause(IrParseStackItem *items);
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses(IrParseStackItem *items);
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO(IrParseStackItem *items);
ParseTree *else_if_clauses_else_if_clauses_else_if_clause(IrParseStackItem *items);
ParseTree *else_if_clauses_else_if_clause(IrParseStackItem *items);
ParseTree *else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO(IrParseStackItem *items);
ParseTree *else_clause_AR_NOILE_statements_KORO(IrParseStackItem *items);

void errorHandler(TokenLexemePair *erroneousToken);

void printParseTree(ParseTree *tree, int indentationLevel);
void printCodePrefix(FILE *file);
void printCodeSuffix(FILE *file);
void writeCode(FILE *file, ParseTree *root);
void writeCodeForVariablePreparation(FILE *file, ParseTree *variable);

extern char _binary_rulepack_txt_start[];

static ParseTree *tree;
static struct { char *key; int value; } *variables = NULL;

int main(int argc, char **argv) {
	FILE *inputFile;

	FILE *codeFile;
	IrParser *irParser;

	struct string_stream *rulepackStream;

	char outputName[100];

	/* ./<compiler>.out <code_file_path> <output_file_path> <optional -c flag to output in a c file instead of object file> */

	if (argc < 2) {
		fprintf(stderr, "must specify the code file\n");
		fprintf(stderr, "./<compiler>.out <code_file_path> <output_file_path> <optional -c flag to output in a c file instead of object file>\n");
		exit(1);
	}

	if (argc < 3) strcpy(outputName, defaultName".c");
	else sprintf(outputName, "%s.c", argv[2]);

	rulepackStream = string_stream_create(_binary_rulepack_txt_start);

	inputFile = fopen(argv[1], "r");
	codeFile = fopen(outputName, "w");

	irParser = createIrParser(rulepackStream, inputFile, errorHandler,
			start_statements,
			statements_statements_statement,
			statements_statement,
			statement_variable_ASSIGNOP_logic_expression,
			statement_PORO_variable,
			statement_LIKHO_logic_expression,
			statement_loop,
			statement_conditional,
			logic_expression_rel_expression,
			logic_expression_rel_expression_LOGICOP_rel_expression,
			logic_expression_logic_expression_NA,
			rel_expression_simple_expression,
			rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP,
			rel_expression_rel_expression_NA,
			simple_expression_term,
			simple_expression_simple_expression_ADDOP_term,
			term_unary_expression,
			term_term_MULOP_unary_expression,
			unary_expression_ADDOP_unary_expression,
			unary_expression_factor,
			factor_variable,
			factor_NUMBER,
			factor_LPAREN_logic_expression_RPAREN,
			variable_ID,
			variable_ID_LTHIRD_logic_expression_RTHIRD,
			loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO,
			conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause,
			conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause,
			conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses,
			conditional_JODI_logic_expression_HOY_TAILE_statements_KORO,
			else_if_clauses_else_if_clauses_else_if_clause,
			else_if_clauses_else_if_clause,
			else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO,
			else_clause_AR_NOILE_statements_KORO
				);

	sh_new_strdup(variables);
	irParse(irParser);

	/*printf("\n\n\n");*/
	/*printParseTree(tree, 0);*/

	printCodePrefix(codeFile);
	writeCode(codeFile, tree);
	printCodeSuffix(codeFile);

	shfree(variables);
	destroyParseTree(tree);
	destroyIrParser(irParser);
	fclose(codeFile);
	fclose(inputFile);
	string_stream_close(rulepackStream);

	if (argc < 4) {
		char command[300];
		sprintf(command, "gcc %s.c -o %s", argc < 3? defaultName : argv[2], argc < 3? "a" : argv[2]);
		system(command);
		sprintf(command, "rm %s", outputName);
		system(command);
	}


	return 0;
}


ParseTree *start_statements(IrParseStackItem *items) {
	Node node;
	node.rule = e_start_statements;

	/*puts("start: statements");*/

	tree = createParseTree(node, NULL, 1, items[0].value);

	return tree;
}
ParseTree *statements_statements_statement(IrParseStackItem *items) {
	Node node;
	node.rule = e_statements_statements_statement;

	/*puts("statements: statements_statement");*/

	return createParseTree(node, NULL, 2, items[0].value, items[1].value);
}
ParseTree *statements_statement(IrParseStackItem *items) {
	Node node;
	node.rule = e_statements_statement;

	/*puts("statements: statement");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *statement_variable_ASSIGNOP_logic_expression(IrParseStackItem *items) {
	Node node;
	node.rule = e_statement_variable_ASSIGNOP_logic_expression;

	/*puts("statement: variable ASSIGNOP logic expression");*/

	return createParseTree(node, NULL, 2, items[0].value, items[2].value);
}
ParseTree *statement_PORO_variable(IrParseStackItem *items) {
	Node node;
	node.rule = e_statement_PORO_variable;

	/*puts("statement: PORO variable");*/

	return createParseTree(node, NULL, 1, items[1].value);
}
ParseTree *statement_LIKHO_logic_expression(IrParseStackItem *items) {
	Node node;
	node.rule = e_statement_LIKHO_logic_expression;

	/*puts("statement: LIKHO logic_expression");*/

	return createParseTree(node, NULL, 1, items[1].value);
}
ParseTree *statement_loop(IrParseStackItem *items) {
	Node node;
	node.rule = e_statement_loop;

	/*puts("statement: loop");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *statement_conditional(IrParseStackItem *items) {
	Node node;
	node.rule = e_statement_conditional;

	/*puts("statement: conditional");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *logic_expression_rel_expression(IrParseStackItem *items) {
	Node node;
	node.rule = e_logic_expression_rel_expression;

	/*puts("logic_expression: rel_expression");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *logic_expression_rel_expression_LOGICOP_rel_expression(IrParseStackItem *items) {
	Node node;
	Node logicopNode;
	ParseTree *logicopParseTree;

	node.rule = e_logic_expression_rel_expression_LOGICOP_rel_expression;

	strcpy(logicopNode.lexVal, items[1].lexVal);
	logicopParseTree = createParseTree(logicopNode, NULL, 0);

	/*puts("logic_expression: rel_expression LOGICOP rel_expression");*/

	return createParseTree(node, NULL, 3, items[0].value, logicopParseTree, items[2].value);
}
ParseTree *logic_expression_logic_expression_NA(IrParseStackItem *items) {
	Node node;
	node.rule = e_logic_expression_logic_expression_NA;

	/*puts("logic_expression: logic_expression NA");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *rel_expression_simple_expression(IrParseStackItem *items) {
	Node node;
	node.rule = e_rel_expression_simple_expression;

	/*puts("rel_expression: simple_expression");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP(IrParseStackItem *items) {
	Node node;
	Node relopNode;
	ParseTree *relopParseTree;

	node.rule = e_rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP;

	strcpy(relopNode.lexVal, items[4].lexVal);
	relopParseTree = createParseTree(relopNode, NULL, 0);

	/*puts("rel_expression: simple_expression COMMA simple_expression ER RELOP");*/

	return createParseTree(node, NULL, 3, items[0].value, items[2].value, relopParseTree);
}
ParseTree *rel_expression_rel_expression_NA(IrParseStackItem *items) {
	Node node;
	node.rule = e_rel_expression_rel_expression_NA;

	/*puts("rel_expression: rel_expression NA");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *simple_expression_term(IrParseStackItem *items) {
	Node node;
	node.rule = e_simple_expression_term;

	/*puts("simple_expression: term");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *simple_expression_simple_expression_ADDOP_term(IrParseStackItem *items) {
	Node node;
	Node addopNode;
	ParseTree *addopParseTree;

	node.rule = e_simple_expression_simple_expression_ADDOP_term;

	strcpy(addopNode.lexVal, items[1].lexVal);
	addopParseTree = createParseTree(addopNode, NULL, 0);

	/*puts("simple_expression: simple_expression ADDOP term");*/

	return createParseTree(node, NULL, 3, items[0].value, addopParseTree, items[2].value);
}
ParseTree *term_unary_expression(IrParseStackItem *items) {
	Node node;
	node.rule = e_term_unary_expression;

	/*puts("term: unary_expression");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *term_term_MULOP_unary_expression(IrParseStackItem *items) {
	Node node;
	Node mulopNode;
	ParseTree *mulopParseTree;

	node.rule = e_term_term_MULOP_unary_expression;

	strcpy(mulopNode.lexVal, items[1].lexVal);
	mulopParseTree = createParseTree(mulopNode, NULL, 0);

	/*puts("term: term MULOP unary_expression");*/

	return createParseTree(node, NULL, 3, items[0].value, mulopParseTree, items[2].value);
}
ParseTree *unary_expression_ADDOP_unary_expression(IrParseStackItem *items) {
	Node node;
	Node addopNode;
	ParseTree *addopParseTree;

	node.rule = e_unary_expression_ADDOP_unary_expression;

	strcpy(addopNode.lexVal, items[0].lexVal);
	addopParseTree = createParseTree(addopNode, NULL, 0);

	/*puts("unary_expression: ADDOP unary_expression");*/

	return createParseTree(node, NULL, 2, addopParseTree, items[1].value);
}
ParseTree *unary_expression_factor(IrParseStackItem *items) {
	Node node;
	node.rule = e_unary_expression_factor;

	/*puts("unary_expression: factor");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *factor_variable(IrParseStackItem *items) {
	Node node;
	node.rule = e_factor_variable;

	/*puts("factor: variable");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *factor_NUMBER(IrParseStackItem *items) {
	Node node;
	Node numberNode;
	ParseTree *numberParseTree;

	node.rule = e_factor_NUMBER;

	strcpy(numberNode.lexVal, items[0].lexVal);
	numberParseTree = createParseTree(numberNode, NULL, 0);

	/*puts("factor: NUMBER");*/

	return createParseTree(node, NULL, 1, numberParseTree);
}
ParseTree *factor_LPAREN_logic_expression_RPAREN(IrParseStackItem *items) {
	Node node;
	node.rule = e_factor_LPAREN_logic_expression_RPAREN;

	/*puts("factor: LPAREN logic_expression REPAREN");*/

	return createParseTree(node, NULL, 1, items[1].value);
}
ParseTree *variable_ID(IrParseStackItem *items) {
	Node node;
	Node idNode;
	ParseTree *idParseTree;

	node.rule = e_variable_ID;

	strcpy(idNode.lexVal, items[0].lexVal);
	idParseTree = createParseTree(idNode, NULL, 0);

	/*puts("variable: ID");*/

	shput(variables, idNode.lexVal, 1);

	return createParseTree(node, NULL, 1, idParseTree);
}
ParseTree *variable_ID_LTHIRD_logic_expression_RTHIRD(IrParseStackItem *items) {
	Node node;
	Node idNode;
	ParseTree *idParseTree;

	node.rule = e_variable_ID_LTHIRD_logic_expression_RTHIRD;

	strcpy(idNode.lexVal, items[0].lexVal);
	idParseTree = createParseTree(idNode, NULL, 0);

	/*puts("variable: ID LTHIRD logic_expression RTHIRD");*/

	shput(variables, idNode.lexVal, 1);

	return createParseTree(node, NULL, 2, idParseTree, items[2].value);
}
ParseTree *loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO(IrParseStackItem *items) {
	Node node;
	node.rule = e_loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO;

	/*puts("loop: JOTOKKHON logic_expression THAKE TOTOKKHON statements KORO");*/

	return createParseTree(node, NULL, 2, items[1].value, items[4].value);
}
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause(IrParseStackItem *items) {
	Node node;
	node.rule = e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause;

	/*puts("conditional: JODI logic_expression HOY TAILE statements KORO else_if_clauses else_clause");*/

	return createParseTree(node, NULL, 4, items[1].value, items[4].value, items[6].value, items[7].value);
}
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause(IrParseStackItem *items) {
	Node node;
	node.rule = e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause;

	/*puts("conditional: JODI logic_expression HOY TAILE statements KORO else_clause");*/

	return createParseTree(node, NULL, 3, items[1].value, items[4].value, items[6].value);
}
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses(IrParseStackItem *items) {
	Node node;
	node.rule = e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses;

	/*puts("conditional: JODI logic_expression HOY TAILE statements KORO else_if_clause");*/

	return createParseTree(node, NULL, 3, items[1].value, items[4].value, items[6].value);
}
ParseTree *conditional_JODI_logic_expression_HOY_TAILE_statements_KORO(IrParseStackItem *items) {
	Node node;
	node.rule = e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO;

	/*puts("conditional: JODI logic_expression HOY TAILE statements KORO");*/

	return createParseTree(node, NULL, 2, items[1].value, items[4].value);
}
ParseTree *else_if_clauses_else_if_clauses_else_if_clause(IrParseStackItem *items) {
	Node node;
	node.rule = e_else_if_clauses_else_if_clauses_else_if_clause;

	/*puts("else_if_clauses: else_if_clauses else_if_clause");*/

	return createParseTree(node, NULL, 2, items[0].value, items[1].value);
}
ParseTree *else_if_clauses_else_if_clause(IrParseStackItem *items) {
	Node node;
	node.rule = e_else_if_clauses_else_if_clause;

	/*puts("else_if_clauses: else_if_clause");*/

	return createParseTree(node, NULL, 1, items[0].value);
}
ParseTree *else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO(IrParseStackItem *items) {
	Node node;
	node.rule = e_else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO;

	/*puts("else_if_clause: NOILE JODI logic_expression HOY TAILE statements KORO");*/

	return createParseTree(node, NULL, 2, items[2].value, items[5].value);
}
ParseTree *else_clause_AR_NOILE_statements_KORO(IrParseStackItem *items) {
	Node node;
	node.rule = e_else_clause_AR_NOILE_statements_KORO;

	/*puts("else_clause: AR NOILE statements KORO");*/

	return createParseTree(node, NULL, 1, items[2].value);
}




void errorHandler(TokenLexemePair *erroneousToken) {
	puts("Synatx error");
	printf("At %s in line number %d\n", erroneousToken->lexeme, erroneousToken->lineNumber + 1);
	free(erroneousToken);
}

void printParseTree(ParseTree *root, int indentationLevel) {
	int indentation;
	for (indentation = 0; indentation < indentationLevel; ++indentation) {
		printf(" ");
	}

	if (root->childrenCount == 0) {
		printf("%s\n", root->content.lexVal);
	}
	else {
		Rule rule = root->content.rule;
		if (rule == e_start_statements) {
			printf("e_start_statements\n");
		}
		if (rule == e_statements_statements_statement) {
			printf("e_statements_statements_statement\n");
		}
		if (rule == e_statements_statement) {
			printf("e_statements_statement\n");
		}
		if (rule == e_statement_variable_ASSIGNOP_logic_expression) {
			printf("e_statement_variable_ASSIGNOP_logic_expression\n");
		}
		if (rule == e_statement_PORO_variable) {
			printf("e_statement_PORO_variable\n");
		}
		if (rule == e_statement_LIKHO_logic_expression) {
			printf("e_statement_LIKHO_logic_expression\n");
		}
		if (rule == e_statement_loop) {
			printf("e_statement_loop\n");
		}
		if (rule == e_statement_conditional) {
			printf("e_statement_conditional\n");
		}
		if (rule == e_logic_expression_rel_expression) {
			printf("e_logic_expression_rel_expression\n");
		}
		if (rule == e_logic_expression_rel_expression_LOGICOP_rel_expression) {
			printf("e_logic_expression_rel_expression_LOGICOP_rel_expression\n");
		}
		if (rule == e_logic_expression_logic_expression_NA) {
			printf("e_logic_expression_logic_expression_NA\n");
		}
		if (rule == e_rel_expression_simple_expression) {
			printf("e_rel_expression_simple_expression\n");
		}
		if (rule == e_rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP) {
			printf("e_rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP\n");
		}
		if (rule == e_rel_expression_rel_expression_NA) {
			printf("e_rel_expression_rel_expression_NA\n");
		}
		if (rule == e_simple_expression_term) {
			printf("e_simple_expression_term\n");
		}
		if (rule == e_simple_expression_simple_expression_ADDOP_term) {
			printf("e_simple_expression_simple_expression_ADDOP_term\n");
		}
		if (rule == e_term_unary_expression) {
			printf("e_term_unary_expression\n");
		}
		if (rule == e_term_term_MULOP_unary_expression) {
			printf("e_term_term_MULOP_unary_expression\n");
		}
		if (rule == e_unary_expression_ADDOP_unary_expression) {
			printf("e_unary_expression_ADDOP_unary_expression\n");
		}
		if (rule == e_unary_expression_factor) {
			printf("e_unary_expression_factor\n");
		}
		if (rule == e_factor_variable) {
			printf("e_factor_variable\n");
		}
		if (rule == e_factor_NUMBER) {
			printf("e_factor_NUMBER\n");
		}
		if (rule == e_factor_LPAREN_logic_expression_RPAREN) {
			printf("e_factor_LPAREN_logic_expression_RPAREN\n");
		}
		if (rule == e_variable_ID) {
			printf("e_variable_ID\n");
		}
		if (rule == e_variable_ID_LTHIRD_logic_expression_RTHIRD) {
			printf("e_variable_ID_LTHIRD_logic_expression_RTHIRD\n");
		}
		if (rule == e_loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO) {
			printf("e_loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO\n");
		}
		if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause) {
			printf("e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause\n");
		}
		if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause) {
			printf("e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause\n");
		}
		if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses) {
			printf("e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses\n");
		}
		if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO) {
			printf("e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO\n");
		}
		if (rule == e_else_if_clauses_else_if_clauses_else_if_clause) {
			printf("e_else_if_clauses_else_if_clauses_else_if_clause\n");
		}
		if (rule == e_else_if_clauses_else_if_clause) {
			printf("e_else_if_clauses_else_if_clause\n");
		}
		if (rule == e_else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO) {
			printf("e_else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO\n");
		}
		if (rule == e_else_clause_AR_NOILE_statements_KORO) {
			printf("e_else_clause_AR_NOILE_statements_KORO\n");
		}
	}

	{
		int childIndex;
		for (childIndex = 0; childIndex < root->childrenCount; ++childIndex) {
			printParseTree(root->children[childIndex], indentationLevel + 1);
		}
	}
}

void printCodePrefix(FILE *file) {
	fprintf(file, 
			"#include <stdio.h>\n"
			"#include <stdlib.h>\n"
			"typedef struct {\n"
			"int *array;\n"
			"int length;\n"
			"int capacity;\n"
			"} IntArrayList;\n"
			"IntArrayList *createIntArrayList(void);\n"
			"void appendInIntArrayList(IntArrayList *list, int value);\n"
			"void destroyIntArrayList(IntArrayList *list);\n"
			"int main(void) {\n"
			);
	{
		int i;
		for (i = 0; i < shlen(variables); ++i) {
			fprintf(file, "IntArrayList *%s = createIntArrayList();\n", variables[i].key);
		}
	}
}

void printCodeSuffix(FILE *file) {
	{
		int i;
		for (i = 0; i < shlen(variables); ++i) {
			fprintf(file, "destroyIntArrayList(%s);\n", variables[i].key);
		}
	}

	fprintf(file, 
			"return 0;\n"
			"}\n"
			"IntArrayList *createIntArrayList(void) {\n"
			"IntArrayList *list = (IntArrayList *)malloc(sizeof(IntArrayList));\n"
			"list->length = 0;\n"
			"list->capacity = 8;\n"
			"list->array = (int *)malloc(list->capacity * sizeof(int));\n"
			"return list;\n"
			"}\n");
	fprintf(file,
			"void appendInIntArrayList(IntArrayList *list, int value) {\n"
			"if (list->length + 1 >= list->capacity) {\n"
			"list->capacity *= 2;\n"
			"{\n"
			"int *copiedArray = (int *)malloc(list->capacity * sizeof(int));\n"
			"int i;\n"
			"for (i = 0; i < list->length; ++i) {\n"
			"copiedArray[i] = list->array[i];\n"
			"}\n"
			"free(list->array);\n"
			"list->array = copiedArray;\n"
			"}\n"
			"}\n"
			"list->array[list->length++] = value;\n"
			"}\n"
			);
	fprintf(file, 
			"void destroyIntArrayList(IntArrayList *list) {\n"
			"free(list->array);\n"
			"free(list);\n"
			"}\n"
			);
}

void writeCode(FILE *file, ParseTree *root) {
	Node node = root->content;
	Rule rule = node.rule;

	if (rule == e_start_statements) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_statements_statements_statement) {
		writeCode(file, root->children[0]);
		writeCode(file, root->children[1]);
	}
	else if (rule == e_statements_statement) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_statement_variable_ASSIGNOP_logic_expression) {
		writeCodeForVariablePreparation(file, root->children[0]);
		writeCode(file, root->children[0]);
		fprintf(file, " = ");
		writeCode(file, root->children[1]);
		fprintf(file, ";\n");
	}
	else if (rule == e_statement_PORO_variable) {
		writeCodeForVariablePreparation(file, root->children[0]);
		fprintf(file, "scanf(\"%%d\", &");
		writeCode(file, root->children[0]);
		fprintf(file, ");\n");
	}
	else if (rule == e_statement_LIKHO_logic_expression) {
		fprintf(file, "printf(\"%%d\\n\", ");
		writeCode(file, root->children[0]);
		fprintf(file, ");\n");
	}
	else if (rule == e_statement_loop) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_statement_conditional) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_logic_expression_rel_expression) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_logic_expression_rel_expression_LOGICOP_rel_expression) {
		writeCode(file, root->children[0]);
		if (strcmp(root->children[1]->content.lexVal, "othoba") == 0) fprintf(file, " || ");
		else fprintf(file, " && ");
		writeCode(file, root->children[2]);
	}
	else if (rule == e_logic_expression_logic_expression_NA) {
		fprintf(file, "!(");
		writeCode(file, root->children[0]);
		fprintf(file, ")");
	}
	else if (rule == e_rel_expression_simple_expression) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_rel_expression_simple_expression_COMMA_simple_expression_ER_RELOP) {
		writeCode(file, root->children[0]);
		if (strcmp(root->children[2]->content.lexVal, "boro") == 0) fprintf(file, " > ");
		else if (strcmp(root->children[2]->content.lexVal, "choto") == 0) fprintf(file, " < ");
		else fprintf(file, " == ");
		writeCode(file, root->children[1]);
	}
	else if (rule == e_rel_expression_rel_expression_NA) {
		fprintf(file, "!(");
		writeCode(file, root->children[0]); 
		fprintf(file, ")");
	}
	else if (rule == e_simple_expression_term) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_simple_expression_simple_expression_ADDOP_term) {
		writeCode(file, root->children[0]);
		fprintf(file, "%s", root->children[1]->content.lexVal);
		writeCode(file, root->children[2]);
	}
	else if (rule == e_term_unary_expression) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_term_term_MULOP_unary_expression) {
		writeCode(file, root->children[0]);
		if (strcmp(root->children[1]->content.lexVal, "vagshesh") == 0) fprintf(file, " %% ");
		else fprintf(file, "%s", root->children[1]->content.lexVal);
		writeCode(file, root->children[2]);
	}
	else if (rule == e_unary_expression_ADDOP_unary_expression) {
		fprintf(file, "%s", root->children[0]->content.lexVal);
		writeCode(file, root->children[1]);
	}
	else if (rule == e_unary_expression_factor) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_factor_variable) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_factor_NUMBER) {
		fprintf(file, "%s", root->children[0]->content.lexVal);
	}
	else if (rule == e_factor_LPAREN_logic_expression_RPAREN) {
		fprintf(file, "(");
		writeCode(file, root->children[0]);
		fprintf(file, ")");
	}
	else if (rule == e_variable_ID) {
		fprintf(file, "%s->array[0]", root->children[0]->content.lexVal);
	}
	else if (rule == e_variable_ID_LTHIRD_logic_expression_RTHIRD) {
		fprintf(file, "%s->array[", root->children[0]->content.lexVal);
		writeCode(file, root->children[1]);
		fprintf(file, "]");
	}
	else if (rule == e_loop_JOTOKKHON_logic_expression_THAKE_TOTOKKHON_statements_KORO) {
		fprintf(file, "while (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
	}
	else if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses_else_clause) {
		fprintf(file, "if (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
		writeCode(file, root->children[2]);
		writeCode(file, root->children[3]);
	}
	else if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_clause) {
		fprintf(file, "if (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
		writeCode(file, root->children[2]);
	}
	else if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO_else_if_clauses) {
		fprintf(file, "if (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
		writeCode(file, root->children[2]);
	}
	else if (rule == e_conditional_JODI_logic_expression_HOY_TAILE_statements_KORO) {
		fprintf(file, "if (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
	}
	else if (rule == e_else_if_clauses_else_if_clauses_else_if_clause) {
		writeCode(file, root->children[0]);
		writeCode(file, root->children[1]);
	}
	else if (rule == e_else_if_clauses_else_if_clause) {
		writeCode(file, root->children[0]);
	}
	else if (rule == e_else_if_clause_NOILE_JODI_logic_expression_HOY_TAILE_statements_KORO) {
		fprintf(file, "else if (");
		writeCode(file, root->children[0]);
		fprintf(file, ") {\n");
		writeCode(file, root->children[1]);
		fprintf(file, "}\n");
	}
	else if (rule == e_else_clause_AR_NOILE_statements_KORO) {
		fprintf(file, "else {\n");
		writeCode(file, root->children[0]);
		fprintf(file, "}\n");
	}

}

void writeCodeForVariablePreparation(FILE *file, ParseTree *variable) {
	Node node = variable->content;
	Rule rule = node.rule;
	char *name = variable->children[0]->content.lexVal;
	if (rule == e_variable_ID) {
		fprintf(file, "while (%s->length < 1) appendInIntArrayList(%s, 0);\n", name, name);
	}
	else if (rule == e_variable_ID_LTHIRD_logic_expression_RTHIRD) {
		fprintf(file, "while (%s->length < ", name);
		writeCode(file, variable->children[1]);
		fprintf(file, " + 1) appendInIntArrayList(%s, 0);\n", name);
	}
}
