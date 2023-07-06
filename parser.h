#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "lexer.h"

enum AstNodeType {
	UNDEF_NODE,
	BIN_MSG_NODE,
	INT_NODE,
};

#define AST_NODE_CLASS enum AstNodeType type

typedef struct {
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
