#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "debug.h"
#include "self_vm.h"

#define PROMPT_LEN_MAX 4096
#define MAX_CMD_SIZE 10

static size_t strnlen_smallest(char *s1, char *s2, size_t max_len)
{
    return MIN(strnlen(s1, max_len), strnlen(s2, max_len));
}

static bool str_eq(char *s1, char *s2)
{
    return strncmp(s1, s2, strnlen_smallest(s1, s2, MAX_CMD_SIZE)) == 0;
}

int main()
{
    SelfVmCode *code = self_vm_code_create();
    char *prompt = malloc(PROMPT_LEN_MAX);

    printf("> ");
    while(fgets(prompt, PROMPT_LEN_MAX, stdin)) {
        char *word = strtok(prompt, " ");
        if (str_eq(word, "exit")) {
            puts("Exiting...");
            break;
        } else if (str_eq(word, "insert")) {
            char *instr = strtok(NULL, " ");
            if (str_eq(instr, "MSG_SEND")) {
                char *msg = strtok(NULL, "\n");
                if (msg == NULL) {
                    printf("Error. Expected message to follow opcode\n");
                } else {
d_printf("%s (line %d): msg is '%s'\n", __func__, __LINE__, msg);

                    self_vm_code_insert_instr(code, MSG_SEND);
                    self_vm_code_insert_str(code, msg);
                }
            } else if (str_eq(instr, "NEW_INT")) {
                char *endptr;
                char *int_str = strtok(NULL, " "); 
                int_str[strnlen(int_str, PROMPT_LEN_MAX) - 1] = '\0';
                long int32 = strtol(int_str, &endptr, 10);
                if (*int_str != '\0' && *endptr == '\0') {
d_printf("%s (line %d): Inserting NEW_INT instruction\n", __func__, __LINE__);
                    self_vm_code_insert_instr(code, NEW_INT);
d_printf("%s (line %d): Inserting integer %d\n", __func__, __LINE__, int32);
                    self_vm_code_insert_int32(code, int32);
                } else {
                    printf("Error. Invalid number '%s'\n", int_str);
                }
            } else {
                printf("Unknown instruction %s\n", instr);
            }
        } else if (str_eq(word, "show")) {
            print_self_vm_code(code);
        } else {
            printf("Unrecognized_command %s\n", word);
        }

        printf("> ");
    }

    self_vm_code_destroy(code);
    return -1;
}
