#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "lexer.h"
#include "self_vm.h"

#define AST_NODE_CLASS void (*codegen)(struct ast_node *self, SelfVmCode *code)

typedef struct ast_node {
	AST_NODE_CLASS;
} AstNode;

typedef struct {
	AST_NODE_CLASS;
	AstNode *rcvr;
	Symbol *bin_op;
	AstNode *arg;
} BinMsgAstNode;

typedef struct {
	AST_NODE_CLASS;
	Symbol *value;
} IntAstNode;

AstNode *parse_prog(FILE *text_stream);

#endif // PARSER_H
