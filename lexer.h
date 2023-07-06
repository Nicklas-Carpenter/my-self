#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define LEX_BUF_SIZE 64

enum SymType {
	UNDEF_SYM,
	EOS,
	INT_SYM,
	BIN_OP_SYM
};

union SymVal {
	int as_int;
	char as_bin_op;
};

typedef struct {
	enum SymType type;
	union SymVal value;
} Symbol;

typedef struct {
    Symbol *lookahead_sym;
    FILE *text_stream;
    char lex_buf[LEX_BUF_SIZE];
} LexerContext;

LexerContext *lexer_new(FILE *text_stream);
void lexer_destroy(LexerContext *ctx);

Symbol *lexer_lookahead_sym(LexerContext *ctx);
Symbol *lexer_next_sym(LexerContext* ctx);

#endif // LEXER_H
