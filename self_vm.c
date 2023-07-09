#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "self_vm.h"
#include "debug.h"

SelfVmCode *self_vm_code_alloc(uint8_t size_expo)
{
    SelfVmCode *new_code = malloc(sizeof(SelfVmCode));
    if (new_code == NULL)
        return NULL;

    *new_code = (SelfVmCode){
        .size_expo = size_expo,
        .start = malloc(1<<size_expo),
        .insert_offset = 0,
        .read_offset = 0
    };
    if (new_code->start == NULL)
        return NULL;

    return new_code;
}

SelfVmCode *self_vm_code_create()
{
    return self_vm_code_alloc(SELF_VM_CODE_DEFAULT_SIZE_EXPO);
}

void self_vm_code_destroy(SelfVmCode *code)
{
    free(code->start);
    free(code);
}

static inline int self_vm_code_resize(SelfVmCode *code)
{
    code->start = realloc(code->start, 1<<(++code->size_expo));
    if (code->start == NULL)
        return -1;

    return 0;
}

static int insert_data(SelfVmCode *code, void *data, size_t size)
{
d_printf("%s (line %d): inserting %d bytes of data\n", __func__, __LINE__, size);
    // If the number of bytes remaining is less than the size of the data we
    // want to insert, we need to resize before insertion.
    if (code->insert_offset + size > 1<<(code->size_expo)) {
d_printf("%s (line %d): Not enough space to insert data. Resizing...\n", __func__, __LINE__);
        // If resizing fails, indicate an error.
        if (self_vm_code_resize(code))
            return -1;

    }

d_printf("%s (line %d): insert offset = %d\n", __func__, __LINE__, code->insert_offset);
    memcpy((code->start + code->insert_offset), data, size);
d_printf("%s (line %d): Updating offset\n", __func__, __LINE__);
    code->insert_offset += size;
    return 0;

}


int self_vm_code_insert_instr(SelfVmCode *code, uint8_t instr)
{
    return insert_data(code, (uint8_t *)(&instr), sizeof(uint8_t));
}


int self_vm_code_insert_int32(SelfVmCode *code, int32_t int32)
{
d_printf("%s (line %d): inserting value %d\n",  __func__, __LINE__, int32);
    return insert_data(code, &int32, sizeof(int32_t));
}

int self_vm_code_insert_str(SelfVmCode *code, char *str_ptr)
{
    return insert_data(code, &str_ptr, sizeof(char *));
}

int get_next_data(SelfVmCode *code, void *dst, size_t size)
{
    if (code->read_offset + size > code->insert_offset)
        return -1;

d_printf("%s (line %d): read offset = %d\n", __func__, __LINE__, code->read_offset);
   memcpy(dst, (code->start + code->read_offset), size);
   code->read_offset += size;
   return 0;
}

int self_vm_code_get_next_instr(SelfVmCode *code, uint8_t *instr_p)
{
    return get_next_data(code, instr_p, sizeof(uint8_t));
}

int self_vm_code_get_next_int32(SelfVmCode *code, int32_t *int_p)
{
    return get_next_data(code, int_p, sizeof(int32_t));
}

int self_vm_code_get_next_str(SelfVmCode *code, char **str_p)
{
    return get_next_data(code, str_p, sizeof(char *));
}

bool self_vm_code_data_remaining(SelfVmCode *code)
{
    return code->read_offset < code->insert_offset;
}

// Set the read offset in the code. Returns -1 to indicate error if the given
// offset exceeds the size of the code. Returns 0 on success.
int self_vm_code_seek(SelfVmCode *code, ptrdiff_t offset)
{
    if (offset > (1<<(code->size_expo)))
        return -1;

    code->read_offset = offset;
    return 0;
}

void print_self_vm_code(SelfVmCode *code)
{
    while (self_vm_code_data_remaining(code)) {
        uint8_t instr;
        self_vm_code_get_next_instr(code, &instr);

        switch(instr) {
        case MSG_SEND:
            char *msg;
            if (self_vm_code_get_next_str(code, &msg) < 0) {
                puts("Error. No str ptr after MSG_SEND opcode");
                return;
            }

            printf("MSG_SEND %s\n", msg);
            continue;
        case NEW_INT:
            int int_val;
            if (self_vm_code_get_next_int32(code, &int_val) < 0) {
                puts("Error. No int value after NEW_INT opcode");
                return;
            }

            printf("NEW_INT %d\n", int_val);
            continue;
        default:
            printf("Unknown instruction %d\n", instr);
        }
    }
}
