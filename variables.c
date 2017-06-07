#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "region.h"
#include "reporting.h"
#include "stringport.h"
#include "tokenizer.h"
#include "variables.h"

char variable_name[TOK_MAX];
char *read_var_error;

char *
expand_variables(region *r, char *tok, int t)
{
	char *stok, *o, *val;
	int alloc_len;
	int i, l;

	stok = tok;
	alloc_len = t+1;
	o = region_malloc(r, alloc_len);

	i = 0;
	while (*tok) {
		if (*tok == '$') {
			if (!(tok = read_variable_prefix(tok))) {
				report("Problem parsing variable inside token [%s] at character [%d]. %s.", stok, i, read_var_error);
				return NULL;
			}

			if (!(val = getenv(variable_name))) {
				report("Reference to an undefined variable inside token [%s] at character [%d]", stok, i);
				return NULL;
			}

			l = strlen(val);
			alloc_len += l;

			if (alloc_len > TOK_MAX) {
				report("Variable expansion blew up token size too large.");
				return NULL;
			}

			o = region_realloc(r, o, alloc_len);
			memcpy(o + i, val, l);
			i += l;
		} else {
			o[i++] = *tok++;
		}
	}
	o[i] = '\0';

	return o;
}

int
variable_character(char c)
{
	return c == '_' || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

char *
read_variable_prefix(char *tok)
{
	int i;
	int bracket;

	assert(*tok == '$');
	tok++;

	// NOTE: We don't bother to bounds check here
	// because tok is already <= the size of a token
	//
	// ...lets see if this ever bites?

	bracket = 0;
	if (*tok == '{') {
		bracket = 1;
		tok++;
	}

	i = 0;
	while (variable_character(*tok)) {
		if (i == 0 && ('0' <= *tok && *tok <= '9')) {
			read_var_error = "var must not start with a digit";
			return NULL;
		}
		variable_name[i++] = *tok++;
	}

	if (bracket && *tok++ != '}') {
		read_var_error = "missing '}'";
		return NULL;
	}

	variable_name[i] = '\0';

	if (!i) {
		read_var_error = "length 0 variable";
		return NULL;
	}

	return tok;
}
