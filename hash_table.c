#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "hash_table.h"

// Allocate memory for a chain entry.
static HashTableEntry *alloc_entry(SelfString *k, SelfObject *v)
{
printf("%s (line %d): Allocating new entry\n", __func__, __LINE__);
    HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
    if (new_entry == NULL)
{
printf("%s (line %d): Failed to allocate space for new entry\n", __func__, __LINE__);
        return NULL;
}

printf("%s (line %d): Initializing new entry\n", __func__, __LINE__);
    *new_entry = (HashTableEntry){.key = k, .value = v, .next = NULL};
printf("%s (line %d): Returning from function\n", __func__, __LINE__);
    return new_entry;
}

// Calculate the index in the table of a given size for the given hash number.
static inline ptrdiff_t get_table_index(hash_t hash, uint8_t size_expo)
{
    // n mod 2<<size_expo but faster
    return (hash & ((1<<size_expo) - 1));
}

static int add_entry(HashTableEntry *entries, uint8_t size_expo, SelfString *k,
                     SelfObject *v)
{
printf("%s (line %d): entries pointer: %p\n", __func__, __LINE__, entries);
    ptrdiff_t idx = get_table_index(k->hash, size_expo);
printf("%s (line %d): Calculated index is %d\n", __func__, __LINE__, idx);

    // Add the entry to the slot in the primary list if available. Otherwise
    // add to the chain of that slot.
    HashTableEntry *entry = entries + idx;
printf("%s (line %d): entry pointer: %p\n", __func__, __LINE__, entry);
    // Allow for "upserts"; i.e. replacement entries
    if (entry->key == NULL || k->hash == entry->key->hash) {
printf("%s (line %d): Inserting into primary slot\n", __func__, __LINE__);
        entry->key = k;
        entry->value = v;
    } else {
printf("%s (line %d): Collision at index %d with (%s, %s)\n", __func__,
        __LINE__, idx, entry->key->raw_str, entry->value->thing);
        while (entry->next != NULL)
            entry = entry->next;

printf("%s (line %d): Allocating new entry\n", __func__, __LINE__);
        entry->next = alloc_entry(k, v);
printf("%s (line %d): Checking validity of new entry\n", __func__, __LINE__);
        if (entry->next == NULL)
{
printf("%s (line %d): Failed to allocate new entry\n", __func__, __LINE__);
            return -1;
}
    }

printf("%s (line %d): Return from function\n", __func__, __LINE__);
    return 0;
}

// Allocate hash table entries.
static inline HashTableEntry *alloc_entries(uint8_t new_size_expo)
{
    size_t table_size = (1<<new_size_expo) * sizeof(HashTableEntry);
    return malloc(table_size);
}

// Allocate a new hash table and corresponding entries. Returns NULL if
// allocation fails otherwise returns the newly allocated table.
HashTable *hash_table_alloc(uint8_t new_size_expo)
{
    // Allocate and zeroize new table.
    HashTable *new_table = malloc(sizeof(HashTable));
    if (new_table == NULL)
        return NULL;

    memset(new_table, 0, 1<<new_size_expo);
    new_table->size_expo = new_size_expo;

    new_table->entries = alloc_entries(new_size_expo);

    return new_table;
}

// Create a new hash table of default size. Returns NULL if allocation fails
// otherwise returns the newly allocated table.
HashTable *hash_table_create() {
    return hash_table_alloc(HASH_TBL_DEFAULT_SIZE_EXPO);
}


static void destroy_chain(HashTableEntry *base_entry)
{
    HashTableEntry *cur = base_entry->next;
    HashTableEntry *next = NULL;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

void hash_table_destroy(HashTable *tbl)
{
    for (ptrdiff_t i = 0; i < (1<<tbl->size_expo); i++) {
        destroy_chain(tbl->entries + i);
    }

    free(tbl);
}

// Resize a hash table and perform necessary reinsertion.
static void hash_table_resize(HashTable *tbl, uint8_t new_size_expo)
{
    HashTableEntry *new_entries = alloc_entries(new_size_expo);

    size_t old_size = 1<<tbl->size_expo;
    for (ptrdiff_t i = 0; i < old_size; i++) {
        HashTableEntry *cur = (tbl->entries + i);

        // Insert non-null entries into the new table.
        if (cur->key != NULL)
            add_entry(new_entries, new_size_expo, cur->key, cur->value);

        // Insert entries along the chain, if any, into the new table and
        // free them. Note that there may be entries in the chain even if the
        // key of the root is NULL.
        cur = cur->next;
        while (cur != NULL) {
            add_entry(new_entries, new_size_expo, cur->key, cur->value);

            HashTableEntry *prev = cur;
            cur = cur->next;
            free(prev);
        }
    }

    tbl->size_expo = new_size_expo;
    free(tbl->entries);
    tbl->entries = new_entries;
}

// Insert the given key-value pair into the given hash table. Returns -1 if we
// are unable to allocate memory for an entry (i.e. if we need to insert it
// into a chain). Otherwise, returns 0.
int hash_table_insert(HashTable *tbl, SelfString *k, SelfObject *v)
{
    if (((tbl->n_entries + 1)>>(tbl->size_expo - 2)) >= 3)
{
printf("%s (line %d): Resizing hash table\n", __func__, __LINE__);
        hash_table_resize(tbl, tbl->size_expo + 1);
}

printf("%s (line %d): Adding entry\n", __func__, __LINE__);
    int status = add_entry(tbl->entries, tbl->size_expo, k, v);
    if (status < 0)
        return -1;

    ++tbl->n_entries;
    return 0;
}

// Get the SelfObject associated with the given key in the given table.
SelfObject *hash_table_get(HashTable *tbl, SelfString *k)
{
printf("%s (line %d): entries pointer: %p\n", __func__, __LINE__, tbl->entries);
    hash_t key_hash = k->hash;
    ptrdiff_t idx = get_table_index(key_hash, tbl->size_expo);
printf("%s (line %d): Calculated index: %d\n", __func__, __LINE__, idx);

    HashTableEntry *cur = (tbl->entries + idx);
printf("%s (line %d): entry pointer: %p\n", __func__, __LINE__, cur);
    while (cur != NULL && cur->key != NULL && key_hash != cur->key->hash)
{
printf("%s (line %d):Searching for match"
            "\n\texpected hash: %s"
            "\n\tcurrent hash: %s\n",
            __func__, __LINE__, key_hash, cur->key->hash);

        cur = cur->next;
}

    // Failed to find requested node, probably because it doesn't exist. Return
    // NULL to indicate error.
    if (cur == NULL || cur->key == NULL)
{
printf("%s (line %d): Null entry at expected position\n", __func__, __LINE__);
        return NULL;
}

    return cur->value;
}

// Remove the SelfObject associated with the given key in the given table. If
// the key does not exist in the table, return -1 otherwise return 0.
int hash_table_remove(HashTable *tbl, SelfString *k)
{
    hash_t key_hash = k->hash;
    ptrdiff_t idx = get_table_index(key_hash, tbl->size_expo);
printf("%s (line %d): Calculated index: %d\n", __func__, __LINE__, idx);

    // Try to find the key either directly in the slot at the calculated index
    // or in its chain.
    HashTableEntry *cur = (tbl->entries + idx);
    HashTableEntry *prev = NULL;
    while (cur != NULL && cur->key != NULL && key_hash != cur->key->hash) {
        prev = cur;
        cur = cur->next;
    }

    // Key not found.
    if (cur == NULL || cur->key == NULL)
        return -1;

    cur->key = NULL;

    // If prev is not NULL and we successfully found a key to remove, the node
    // we are removing is in a chain and we need to free its memory.
    if (prev != NULL) {
        prev->next = cur->next;
        free(cur);
    }

    return 0;
}
