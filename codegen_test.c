#include <stdio.h>

#include "debug.h"
#include "parser.h"
#include "self_vm.h"

int main()
{
    printf("> ");
	AstNode *root = parse_prog(stdin);
    SelfVmCode *code = self_vm_code_create();
    root->codegen(root, code);
    print_self_vm_code(code);

    root->destroy(root);
    self_vm_code_destroy(code);

	return 0;
}
