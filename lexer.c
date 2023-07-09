#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "error.h"
#include "lexer.h"

static int lookahead_char(LexerContext *ctx)
{
    if (ctx->lookahead_char != 0)
        return ctx->lookahead_char;

    ctx->lookahead_char = fgetc(ctx->text_stream);
    return ctx->lookahead_char;
}

static int next_char(LexerContext *ctx)
{
    char lookahead_char = ctx->lookahead_char;
    if (ctx->lookahead_char != 0) {
        ctx->lookahead_char = 0;
        return lookahead_char;
    }

    return fgetc(ctx->text_stream);
}
        

// Read the next few digits from the input stream and copy them to the lexer's
// buffer
static void read_number(LexerContext *ctx, char first)
{
	ctx->lex_buf[0] = first;

	int i = 1;
    while (isdigit((char)lookahead_char(ctx)))
		ctx->lex_buf[i++] = (char)next_char(ctx);

	ctx->lex_buf[i] = '\0';
}

// Create an integer symbol with the given integer value
static Symbol *int_symbol(int value) {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);
	}

	*symbol = (Symbol){
		.type = INT_SYM,
		.value = value,
	};

	return symbol;
}

// Create a binary op symbol with the given operator character
Symbol *bin_op_symbol(char op) {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);
	}

	*symbol = (Symbol){
		.type = BIN_OP_SYM,
		.value = op,
	};

	return symbol;
}

// Return a symbol denoting the end of the stream (EOS)
static Symbol *eos_symbol() {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);
	}
	symbol->type = EOS;

	return symbol;
}

// Get the next symbol. Used for both lookahead and direct use.
static Symbol *next_sym(LexerContext *ctx)
{
	size_t bytes_read;
    int cur_char;

    while ((cur_char = next_char(ctx)) != EOF) {
		if (isspace((char)cur_char)) {
			continue;
		}

		if (isdigit((char)cur_char)) {
			read_number(ctx, cur_char);
			//d_puts("Found INT sym");
			return int_symbol(atoi(ctx->lex_buf));
		} else if ((char)cur_char == '+') {
			//d_puts("Found BIN_OP sym");
			return bin_op_symbol(cur_char);
		} else {
			d_printf("%s (line %d): Unexpected character %c\n",
				__func__, __LINE__, (char)cur_char);
		}
	}

	//d_puts("Reached end of stream");
	return eos_symbol();
}

// Lex and return the next symbol from the input stream but do not overwrite
// the current symbol (yet)
Symbol* lexer_lookahead_sym(LexerContext *ctx)
{
	// Prevent loss of symbol if lookahead_sym is called more than once
	// between calls to next_sym
    Symbol *lookahead_sym = ctx->lookahead_sym;
	if (lookahead_sym == NULL)
		return (ctx->lookahead_sym = next_sym(ctx));

	return lookahead_sym;
}

// Advance the current symbol to the next symbol lexed from the stream.
Symbol *lexer_next_sym(LexerContext *ctx)
{
    // If we previously did a lookahead, then return the lookahead symbol.
    Symbol *lookahead_sym = ctx->lookahead_sym;
	if (lookahead_sym != NULL) {
		ctx->lookahead_sym = NULL;
        return lookahead_sym;
	} else {
        return next_sym(ctx);
	}
}

// Create a new lexer context. Allows the lexer to be reetrant.
LexerContext *lexer_new(FILE *text_stream)
{
    LexerContext *new_ctx = malloc(sizeof(LexerContext));
    *new_ctx = (LexerContext){
        .text_stream = text_stream,
        .lookahead_sym = NULL
    };

    return new_ctx;
}

void lexer_destroy(LexerContext *ctx)
{
    free(ctx);
}
