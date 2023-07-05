#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "self_object.h"
#include "self_string.h"

#define PROMPT_LEN_MAX 4096

bool str_eq(char *s1, char *s2)
{
    return strncmp(s1, s2, strlen(s2)) == 0;
}

static int insert(HashTable *tbl, char *key, char *value)
{
printf("%s (line %d): Creating key SelfString\n", __func__, __LINE__);
    SelfString *k = self_string_create(key);
printf("%s (line %d): Creating value SelfObject\n", __func__, __LINE__);
    SelfObject *v = self_object_create(value);
printf("%s (line %d): Inserting into hash table\n", __func__, __LINE__);
    int rc = hash_table_insert(tbl, k, v);
    if (rc < 0)
{
printf("%s (line %d): Failed to insert entry\n", __func__, __LINE__);
}
printf("%s (line %d): Returning from function\n", __func__, __LINE__);
    return rc;
}

static char *get(HashTable *tbl, char *key)
{
    SelfString *k = self_string_create(key);
    SelfObject *obj = hash_table_get(tbl, k);
    if (obj == NULL)
        return NULL;

    return obj->thing;
}

static int delete(HashTable *tbl, char *key)
{
    SelfString *k = self_string_create(key);
    return hash_table_remove(tbl, k);
}

int main()
{
    HashTable *tbl = hash_table_create();

    char *prompt = malloc(PROMPT_LEN_MAX);
    printf("> ");
    while(fgets(prompt, PROMPT_LEN_MAX, stdin)) {
        // Strip trailing \n
        prompt[strnlen(prompt, PROMPT_LEN_MAX) - 1] = '\0';

        char *word = strtok(prompt, " ");
        if (str_eq(word, "exit")) {
            puts("Exiting...");
            hash_table_destroy(tbl);
            return 0;
        } else if (str_eq(word, "insert")) {
            char *key = strtok(NULL, " ");
printf("%s (line %d): Key string: %s\n", __func__, __LINE__, key);
            char *value = strtok(NULL, " ");
printf("%s (line %d): Value string: %s\n", __func__, __LINE__, value);
            if (key == NULL || value == NULL)
                continue;

printf("%s (line %d): Inserting kv pair\n", __func__, __LINE__);
            if(insert(tbl, key, value))
                puts("Error while attempting to insert kv pair"); 
            else 
                printf("%s --> %s\n", key, value);
        } else if (str_eq(word, "get")) {
            char *key = strtok(NULL, " ");
printf("%s (line %d): Key string: %s\n", __func__, __LINE__, key);
            if (key == NULL)
                continue;

            char *value = get(tbl, key);
            if (value == NULL)
                printf("Unable to find entry with key '%s'\n", key);
            else
                printf("%s\n", value);
        } else if (str_eq(word, "show")) {
            for (ptrdiff_t i = 0; i < (1<<tbl->size_expo); i++) {
                printf("%d:", i);
                HashTableEntry* entry = tbl->entries + i;
                if (entry->key != NULL) {
                    printf("\tKey: %s\n", entry->key->raw_str);
                    printf("\tValue: %s\n\n", entry->value->thing);

                    int j = 1;
                    entry = entry->next;
                    while(entry != NULL) {
                        printf("\t%d.%d:", i, j++);
                        printf("\tKey:%s\n", entry->key->raw_str);
                        printf("\t\tValue: %s\n\n", entry->value->thing);

                        entry = entry->next;
                    }
                } else {
                    puts("\tKey: NULL");
                    puts("\tValue NULL\n");
                }
            }
        } else if (str_eq(word, "remove")) {
            char *key = strtok(NULL, " "); 
            if (delete(tbl, key))
                printf("Unable to remove entry with key '%s'. Likely non-existent", key);
        }

        printf("> ");
    }

    hash_table_destroy(tbl);
    puts("Error reading STDIN. Exiting.");
    return -1;
}
