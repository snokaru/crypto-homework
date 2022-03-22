#include "caesar-cryptanalysis.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char** caesar_generate_all_permutations(char* str, int* len) {
    char *permutations[100];
    int current = 0;
    char* prec = str;

    for (int i = 1; i < strlen(CHARSET); ++i) {
        char* next = (char*)malloc(sizeof(char) * strlen(str));
        strcpy(next, prec);
        for (int j = 0; next[j]; j++) {
            const char* pos = strchr(CHARSET, next[j]);
            if (pos)
                next[j] = CHARSET[(pos - CHARSET + 1) % strlen(CHARSET)];
        }
        permutations[current++] = next;
        prec = next;
    }
    char** result = (char**)malloc(sizeof(char*) * current);
    for (int i = 0; i < current; ++i) {
        result[i] = permutations[i];
    }
    *len = current;
    return result;
}
