#include "polybius.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHARSET "abcdefghiklmnopqrstuvwxyz"
#define MAX_POLYBIUS_CHARS 500

int* polybius_encode(char* text, int* len) {
    int temp[MAX_POLYBIUS_CHARS];
    int current = 0;

    for (int i = 0; text[i]; i++) {
        char current_character;
        if (text[i] == 'j')
            current_character = 'i';
        else
            current_character = text[i];

        char* pos = strchr(CHARSET, current_character);
        if (pos) {
            int actual_pos = pos - CHARSET; 
            int row = actual_pos / 5 + 1;
            int column = actual_pos % 5 + 1;
            temp[current++] = row * 10 + column;
        }
    }
    int* result = (int*)malloc(sizeof(int) * current);
    memcpy(result, temp, sizeof(int) * current);
    *len = current;

    return result;
}
char* polybius_decode(int* numbers, int len) {
    char temp[MAX_POLYBIUS_CHARS];
    int current = 0;

    for (int i = 0; i < len; i++) {
        int row = numbers[i] / 10 - 1;
        int column = (numbers[i] % 10) - 1;
        int actual_pos = row * 5 + column;
        
        temp[current++] = CHARSET[actual_pos];
    }

    temp[current++] = '\0';
    char* result = (char*)malloc(sizeof(char) * current);
    memcpy(result, temp, sizeof(char) * current);

    return result;
}
