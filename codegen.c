#include "parser.h"
#include "self_vm.h"

void codegen_bin_msg_ast_node(AstNode *self, SelfVmCode *code)
{
    BinMsgAstNode *bin_msg = (BinMsgAstNode *)self;
    bin_msg->arg->codegen(bin_msg->arg, code);
    bin_msg->rcvr->codegen(bin_msg->rcvr, code);

    self_vm_code_insert_instr(code, MSG_SEND);
    self_vm_code_insert_str(code, &(bin_msg->bin_op->value.as_bin_op));
}

void codegen_int_ast_node(AstNode *self, SelfVmCode *code)
{
    IntAstNode *integer = (IntAstNode *)self;
    self_vm_code_insert_instr(code, NEW_INT);
    self_vm_code_insert_int32(code, integer->value->value.as_int);
}
