#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "self_vm.h"

void codegen_bin_msg_ast_node(AstNode *self, SelfVmCode *code);
void codegen_int_ast_node(AstNode *self, SelfVmCode *code);

#endif // CODEGEN_H
