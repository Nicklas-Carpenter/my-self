#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

#include "hash.h"
#include "self_object.h"
#include "self_string.h"

#define HASH_TBL_DEFAULT_SIZE_EXPO 4

// typedef'd since it's structure should be considered opague and subject to
// change, and it should only be manipulated through its associated functions
typedef struct hash_table_entry HashTableEntry;

struct hash_table_entry {
    SelfString *key;
    SelfObject *value;
    HashTableEntry *next; // For linear chaining
};

typedef struct {
    HashTableEntry *entries;
    size_t n_entries;
    uint8_t size_expo; // max size is (1<<size_expo)
} HashTable;

HashTable *hash_table_alloc(uint8_t new_size_expo);
HashTable *hash_table_create(void);
void hash_table_destroy(HashTable *tbl);

int hash_table_insert(HashTable *tbl, SelfString *k, SelfObject *v);
SelfObject *hash_table_get(HashTable *tbl, SelfString *k);
int hash_table_remove(HashTable *tbl, SelfString *k);


#endif // HASH_TABLE_H
