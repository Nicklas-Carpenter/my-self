#ifndef SELF_VM_H
#define SELF_VM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "self_object.h"

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

#define SELF_OBJECT_DEFAULT_SIZE_EXPO 3

typedef struct {
    SelfObject *items;
    ptrdiff_t top;
    uint8_t size_expo;
} SelfStack;

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

SelfStack *self_stack_alloc(uint8_t size_expo);
SelfStack *self_stack_create(void)
void self_stack_destroy(SelfStack *stk);

int self_stack_push(SelfStack *stk, SelfObject *obj);
SelfStack *self_stack_pop(SelfStack *stk);

bool self_stack_is_empty(SelfStack *stk);

SelfObject *execute(
#endif // SELF_VM_H
