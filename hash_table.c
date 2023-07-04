#include <stdlib.h>

#include "hash_table.h"

static HashTableEntry *alloc_entry(SelfString *k, Object *v)
{
    HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
    new_entry = {.key = k, .value = v, .next = NULL};
    return new_entry;
}

static inline ptrdiff_t get_table_index(hash_t hash, uint8_t size_expo)
{
    return (hash & ((1<<size_expo) - 1));
}

static void add_entry(HashTableEntry *entries, SelfString *k, Object *v)
{
    ptrdiff_t idx = get_table_index(k->hash, tbl->size_expo);

    HashTableEntry *entry = tbl->entries + idx;
    if (entry->key == NULL) {
        entry.key = k;
        entry.value = v;
    } else {
        while (entry->next != NULL);
            entry = entry->next;

        entry->next = alloc_entry(k, v);
    }


}

static inline HashTableEntry *alloc_entries(uint8_t new_size_expo)
{
    size_t table_size = (1<<new_size_expo) * sizeof(HashTableEntry);
    return malloc(table_size);
}

HashTable *hash_table_alloc(uint8_t new_size_expo)
{
    HashTable *new_table = malloc(sizeof(HashTable));
    memset(new_table, 0, table_size);
    new_table->size_expo = new_size_expo;

    new_table->entries = alloc_entries(new_size_expo);

    return new_table;
}

inline HashTable *hash_table_create() {
    return hash_table_alloc(HASH_TABLE_DEFAULT_SIZE_EXPO);
}

HashTable *hash_table_resize(HashTable *tbl, uint8_t new_size_expo)
{
    HashTableEntry *new_entries = alloc_entries

    size_t old_size = 1<<tbl.size_expo;
    for (size_t i = 0; i < old_size, i++) {
        HashTableEntry *cur = tbl.entries[i];

        // Insert non-null entries into the new table
        if (cur->key != NULL)
            hash_table_insert(new_table, cur->key, cur->value);

        // Insert entries along the chain, if any, into the new table and
        // free them. Note that there may be entries in the chain even if the
        // key of the root is NULL
        cur = cur->next;
        while (cur != NULL) {
            hash_table_insert(new_table, cur->key, cur->value);

            HashTableEntry *prev = cur;
            cur = cur.next;
            free(prev);
        }
    }
}

void hash_table_insert(HashTable *tbl, SelfString *k, Object *v)
{
    if (((tlb->n_entries + 1)>>(tbl->size_expo - 2)) >= 3)
        hash_table_resize(tbl, tbl->size_expo + 1);

    add_entry(tbl->entries, k, v);
    ++tbl->entries;
}

Object *hash_table_get(HashTable *tbl, SelfString *k)
{
    hash_t key_hash = k->hash;
    ptrdiff_t idx = get_table_index(key_hash, tbl->size_expo);

    HashTableEntry *cur = (tbl->entries + idx);
    while (cur != NULL && cur->key != NULL && key_hash != cur->key.hash)
        cur = cur->next;

    // Failed to find requested node, probably because it doesn't exist.
    // Return NULL to indicate error.
    if (cur == NULL || cur->key == NULL)
        return NULL

    return cur->value;
}

int hash_table_remove(HashTable *tbl, SelfString *k)
{
    hash_t key_hash = k->hash;
    ptrdiff_t idx = get_table_index(key_hash, tbl->size_expo);

    HashTableEntry *cur = (tbl->entries + idx);
    HashTableEntry *prev = NULL;
    while (cur != NULL && cur->key != NULL && key_hash != cur->key.hash) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL || cur->key == NULL)
        return -1;

    // If prev is not NULL and we successfully found a key to remove, the node
    // we are removing is in a chain and we need to free its memory.
    if (prev != NULL) {
        prev->next = cur->next;
        free(cur);
    }

    return 0;
}
