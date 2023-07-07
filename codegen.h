#ifndef CODEGEN_H
#define CODEGEN_H

#include "self_vm.h"

void codegen_bin_msg_ast_node(AstNode *self, Code *code);
void codegen_int_ast_node(AstNode *self, Code *code);

#endif // CODEGEN_H
