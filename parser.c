#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "variables.h"
#include "parser.h"

char *operator_for[] = {
	[NODE_PIPE] = "|",
	[NODE_CONJ] = "&&",
	[NODE_DISJ] = "||",
};

void free_ast(struct AST *n)
{
	// TODO
}

char** read_tokens(FILE *f, char **envp)
{
	char **tokens;
	int i, t;

	i = 0;

	tokens = malloc(sizeof(char*)*MAX_TOKS_PER_LINE);

	while ((t = token(f)) != -1) {
		tokens[i] = expand_variables(tok_buf, t, envp);

		i++;
		if (i >= MAX_TOKS_PER_LINE) {
			fprintf(stderr, "Line too long!\n");
			exit(-1);
		}
	}

	tokens[i] = NULL;

	return tokens;
}


struct AST* parse_binop(char **tokens, NodeType ty)
{
	char **stokens;
	struct AST* n;
	struct AST* m;

	stokens = tokens;

	if (ty == NODE_COMMAND) {
		n = malloc(sizeof(struct AST));
		n->type = NODE_COMMAND;
		n->node.tokens = stokens;

		return n;
	}

	while (tokens[0]) {
		if (!strcmp(operator_for[ty], tokens[0])) {
			free(tokens[0]);
			tokens[0] = NULL;

			m = malloc(sizeof(struct AST));
			m->type = ty;
			m->node.child.l = parse_binop(stokens, ty-1);
			m->node.child.r = parse_binop(tokens+1, ty);

			return m;
		}
		else {
			tokens++;
		}
	}

	return parse_binop(stokens, ty-1);
}

struct AST* parse_tokens(char **tokens)
{
	return parse_binop(tokens, NODE_DISJ);
}

struct AST* parse(FILE *f, char **envp)
{
	return parse_tokens(read_tokens(f, envp));
}
