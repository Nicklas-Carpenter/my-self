#include <stdlib.h>

#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include "self_vm.h"

static void codegen_bin_msg_ast_node(Code *code)
{
}

AstNode *int_ast_node(Symbol *sym)
{
	d_puts("Constructing int AST node");
	IntAstNode *node = malloc(sizeof(IntAstNode));
	*node = (IntAstNode){.type = INT_NODE, .value = sym};
	return (AstNode *)node;
}

AstNode *bin_msg_ast_node(AstNode *rcvr, Symbol *op, AstNode *arg)
{
	d_puts("Constructing Binary message AST node");
    d_printf("rcvr = %p; op = %p; arg = %p\n", rcvr, op, arg);
	BinMsgAstNode *node = malloc(sizeof(BinMsgAstNode));
	*node = (BinMsgAstNode){
			.type = BIN_MSG_NODE,
			.rcvr = rcvr,
			.bin_op = op,
			.arg = arg
	};

	d_puts("Return from function");
	return (AstNode *)node;
}

// Parse an expression from input stream
static AstNode *parse_expr(LexerContext *ctx)
{
	d_puts("Parsing expression");
	Symbol* cur_sym = lexer_next_sym(ctx);

	switch(cur_sym->type) {
	case INT_SYM:
		d_puts("Found int symbol");
		if (lexer_lookahead_sym(ctx)->type == BIN_OP_SYM) {
			d_puts("Lookahead is a bin op");
			AstNode *rcvr = int_ast_node(cur_sym);

			cur_sym = lexer_next_sym(ctx);

			AstNode *arg = parse_expr(ctx);

			return bin_msg_ast_node(rcvr, cur_sym, arg);
		}

		d_puts("Not bin op expression");
		return int_ast_node(cur_sym);
	default:
		d_puts("Unrecognized expression");
	}

	return NULL;
}

// Parse the input. Currently we only support expressions.
AstNode *parse_prog(FILE *text_stream)
{
	d_puts("Parsing prog");

    LexerContext *ctx = lexer_new(text_stream);
    AstNode *root = parse_expr(ctx);
    lexer_destroy(ctx);

	return root;
}
