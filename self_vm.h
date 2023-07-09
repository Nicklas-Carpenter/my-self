#ifndef SELF_VM_H
#define SELF_VM_H

#include <stdbool.h>
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
    size_t n_entries;
    uint8_t size_expo;
} SelfVmCode;

SelfVmCode *self_vm_code_alloc(uint8_t size_expo);
SelfVmCode *self_vm_code_create(void);
void self_vm_code_destroy(SelfVmCode *code);

int self_vm_code_insert_instr(SelfVmCode *code, uint8_t instr);
int self_vm_code_insert_int32(SelfVmCode *code, int32_t int32);
int self_vm_code_insert_str(SelfVmCode *code, char *str_ptr);

int self_vm_code_get_next_instr(SelfVmCode *code, uint8_t *instr_p);
int self_vm_code_get_next_int32(SelfVmCode *code, int32_t *int_p);
int self_vm_code_get_next_str(SelfVmCode *code, char **str_p);

bool self_vm_code_data_remaining(SelfVmCode *code);
int self_vm_code_seek(SelfVmCode *code, ptrdiff_t offset);

void print_self_vm_code(SelfVmCode *code);
#endif // SELF_VM_H
