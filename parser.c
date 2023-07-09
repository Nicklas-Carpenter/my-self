#include <stdlib.h>

#include "codegen.h"
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include "self_vm.h"

static void int_ast_node_destroy(AstNode *self)
{
    IntAstNode* int32 = (IntAstNode *)self;
    free(int32->value);
    free(self);
}

static AstNode *int_ast_node_create(Symbol *sym)
{
	//d_puts("Constructing int AST node");
	IntAstNode *node = malloc(sizeof(IntAstNode));
    *node = (IntAstNode){
        .value = sym,
        .codegen = codegen_int_ast_node,
        .destroy = int_ast_node_destroy
    };

	return (AstNode *)node;
}

static void bin_msg_ast_node_destroy(AstNode *self)
{
    BinMsgAstNode *bin_msg = (BinMsgAstNode *)self;

    AstNode *rcvr = bin_msg->rcvr;
    rcvr->destroy(rcvr);

    AstNode *arg = bin_msg->arg;
    arg->destroy(arg);

    free(bin_msg->bin_op);
    free(self);
}

static AstNode *bin_msg_ast_node_create(AstNode *rcvr, Symbol *op, AstNode *arg)
{
	//d_puts("Constructing Binary message AST node");
    //d_printf("rcvr = %p; op = %p; arg = %p\n", rcvr, op, arg);
	BinMsgAstNode *node = malloc(sizeof(BinMsgAstNode));
	*node = (BinMsgAstNode){
            .codegen = codegen_bin_msg_ast_node,
            .destroy = bin_msg_ast_node_destroy,
			.rcvr = rcvr,
			.bin_op = op,
			.arg = arg
	};

	//d_puts("Return from function");
	return (AstNode *)node;
}

// Parse an expression from input stream
static AstNode *parse_expr(LexerContext *ctx)
{ 
    //d_puts("Parsing expression");
	Symbol* cur_sym = lexer_next_sym(ctx);

	switch(cur_sym->type) {
	case INT_SYM:
		//d_puts("Found int symbol");
		if (lexer_lookahead_sym(ctx)->type == BIN_OP_SYM) {
			//d_puts("Lookahead is a bin op");
			AstNode *rcvr = int_ast_node_create(cur_sym);

			cur_sym = lexer_next_sym(ctx);

			AstNode *arg = parse_expr(ctx);

			return bin_msg_ast_node_create(rcvr, cur_sym, arg);
		}

		//d_puts("Not bin op expression");
		return int_ast_node_create(cur_sym);
	default:
		//d_puts("Unrecognized expression");
	}

	return NULL;
}

// Parse the input. Currently we only support expressions.
AstNode *parse_prog(FILE *text_stream)
{
	//d_puts("Parsing prog");

    LexerContext *ctx = lexer_new(text_stream);
    AstNode *root = parse_expr(ctx);
    lexer_destroy(ctx);

	return root;
}
