#ifndef SELF_VM_H
#define SELF_VM_H

#include <stddef.h>
#include <stdint.h>

typedef enum self_vm_opcode {
    MSG_SEND,
    NEW_INT,
    NEW_STR
} SelfVmInstr;

#define SELF_VM_CODE_DEFAULT_SIZE_EXPO 6

typedef struct {
    void *start;
    ptrdiff_t insert_offset;
    ptrdiff_t read_offset;
    uint8_t size_expo;
} SelfVmCode;

SelfVmCode *self_vm_code_alloc(uint8_t size_expo);
SelfVmCode *self_vm_code_create(void);
void self_vm_code_destroy(SelfVmCode *code);

int self_vm_code_insert_instr(SelfVmCode *code, SelfVmInstr instr);
int self_vm_code_insert_int32(SelfVmCode *code, int32_t int32);
int self_vm_code_insert_str_ptr(SelfVmCode *code, char *str_ptr);

SelfVmInstr self_vm_code_get_next_instr(SelfVmCode *code);
int32_t self_vm_code_get_next_int32(SelfVmCode *code);
char *self_vm_code_get_next_str_ptr(SelfVmCode *code);

#endif // SELF_VM_H
