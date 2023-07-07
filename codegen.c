#include "parser.h"
#include "self_vm.h"

void codegen_bin_msg_ast_node(AstNode *self, Code *code)
{
    BinMsgAstNode *bin_msg = (BinMsgAstNode *)self;
    bin_msg->arg->codegen(bin_msg->arg, code);
    bin_msg->rcvr->codegen(bin_msg->rcvr, code);

    self_vm_code_insert_instr(MSG_SEND);
    self_vm_code_insert_str_ptr(&(bin_msg->bin_op->value.as_char));
}

void codegen_int_ast_node(AstNode *self, Code *code)
{
    IntAstNode *integer = (IntAstNode *)self;
    self_vm_code_insert_instr(NEW_INT);
    self_vm_code_insert_int32(integer->value.as_int);
}
