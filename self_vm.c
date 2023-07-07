#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "self_vm.h"

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
    free(code->code_start);
    free(code);
}

static inline int self_vm_code_resize(SelfVmCode *code)
{
    code->code_start = realloc(code->code_start, 1<<(++code->size_expo));
    if (code->code_start == NULL)
        return -1;

    return 0;
}

static int self_vm_code_insert_byte(SelfVmCode *code, uint8_t byte)
{
    // If the number of bytes remaining is less than the size of the data we
    // want to insert, we need to resize before insertion.
    if (code->insert_offset + sizeof(uint8_t) > 1<<(code->size_expo)) {
        // If resizing fails, indicate an error.
        if (self_vm_code_resize(code))
            return -1;
    }

    *((uint8_t *)(code->start + code->insert_offset) = byte;
    code->insert_offset += sizeof(uint8_t);
    return 0;
}

// TODO Do we need to handle the case where resizing does not generate enough
// room for insertion (e.g. size 2 --> size 4 when attempting 8 to insert
// bytes)?
static int self_vm_code_insert_dword(SelfVmCode *code, uint32_t dword)
{
    // If the number of bytes remaining is less than the size of the data we
    // want to insert, we need to resize before insertion.
    if (code->insert_offset + sizeof(uint32_t) > 1<<(code->size_expo)) {
        // If resizing fails, indicate an error.
        if (self_vm_code_resize(code))
            code->insert_offset -= sizeof(uint32_t);
    }

    *((uint32_t *)(code->start + code->insert_offset) = dword;
    code->insert_offset += sizeof(uint32_t);
    return 0;
}

// TODO Do we need to handle the case where resizing does not generate enough
// room for insertion (e.g. size 2 --> size 4 when attempting to insert 8 bytes)?
static int self_vm_code_insert_ptr(SelfVmCode *code, void *ptr)
{
    // If the number of bytes remaining is less than the size of the data we
    // want to insert, we need to resize before insertion.
    if (code->insert_offset + sizeof(void *) > 1<<(code->size_expo)) {
        // If resizing fails, indicate an error.
        if (self_vm_code_resize(code))
            return -1;
    }

    *((void **)(code_start + code->insert_offset) = ptr;
    code->insert_offset += sizeof(void *);
    return 0;
}

int self_vm_code_insert_instr(SelfVmCode *code, SelfVmInstr instr)
{
    return self_vm_code_insert_byte(code, (uint8_t)instr);
}


int self_vm_code_insert_int32(SelfVmCode *code, int32_t int32)
{
    return self_vm_code_insert_dword(code, (uint32_t)int32);
}

int self_vm_code_insert_str_ptr(SelfVmCode *code, char *str_ptr)
{
    return self_vm_code_insert_ptr(code, str_ptr);
}

uint8_t self_vm_code_get_next_byte(SelfVmCode *code)
{
    // Read offset is within data width of insert offset. The number of unread
    // bytes is smaller than the width of the type of data we want to read.
    // Indicate an error.
    if (code->read_offset + sizeof(uint8_t) > code->insert_offset)
        return NULL;

    uint8_t byte = *((uint8_t *)(code->start + code->read_offset++));
    return byte;
}

uint32_t self_vm_code_get_next_dword(SelfVmCode *code)
{
    // Read offset is within data width of insert offset. The number of unread
    // bytes is smaller than the width of the type of data we want to read.
    // Indicate an error.
    if (code->read_offset + sizeof(uint32_t) > code->insert_offset)
        return NULL;

    uint32_t dword = *((uint32_t *)(code->start + code->read_offset++));
    return dword;
}

uint8_t self_vm_code_get_next_ptr(SelfVmCode *code)
{
    // Read offset is within data width of insert offset. The number of unread
    // bytes is smaller than the width of the type of data we want to read.
    // Indicate an error.
    if (code->read_offset + sizeof(void *) > code->insert_offset)
        return NULL;

    void *ptr = *((void **)(code->start + code->read_offset++));
    return ptr;
}

SelfVmInstr self_vm_code_get_next_instr(SelfVmCode *code)
{
    return self_vm_code_get_next_byte(code);
}

int32_t self_vm_code_get_next_int32(SelfVmCode *code)
{
    return (int32_t)self_vm_code_get_new_dword(code);
}

char *self_vm_code_get_next_str_prt(SelfVmCode *code)
{
    return self_vm_code_get_next_ptr(code);
}

bool self_vm_code_data_remaining(Code *code)
{
    return code->read_offset < code->insert_offset;
}

// Set the read offset in the code. Returns -1 to indicate error if the given
// offset exceeds the size of the code. Returns 0 on success.
int self_vm_code_seek(Code *code, ptrdiff_t offset)
{
    if (offset > (1<<(code->size_expo)))
        return -1;

    code->read_offset = offset;
    return 0;
}

void print_self_vm_code(Code *code)
{
    while (self_vm_code_data_remaining(code)) {
        SelfVmInstr instr = self_vm_code_
        switch(
    }
}
