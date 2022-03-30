#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vigenere.h"

#define CHARSET "abcdefghijklmnopqrstuvwxyz"

char* vigenere_encrypt(char* str, char* key) {
    char temp[strlen(str)];
    int k = 0;

    int pos_in_key = 0, key_length = strlen(key);
    for (int i = 0; i < strlen(str); i++) {
        if (strchr(CHARSET, str[i])) {
            short key_pos = strchr(CHARSET, key[pos_in_key]) - CHARSET;
            short char_pos = strchr(CHARSET, str[i]) - CHARSET;

            short char_new_pos = (char_pos + key_pos) % strlen(CHARSET);
            temp[k++] = CHARSET[char_new_pos];
            pos_in_key = (pos_in_key + 1) % key_length;
        }
    }
    temp[k++] = '\0';
    char* encrypted_str = (char*) malloc(sizeof(char) * strlen(temp));
    memcpy(encrypted_str, temp, sizeof(char) * strlen(temp));
    return encrypted_str;

}

char* vigenere_decrypt(char* str, char* key) {
    char temp[strlen(str)];
    int k = 0;

    int pos_in_key = 0, key_length = strlen(key);
    for (int i = 0; i < strlen(str); i++) {
        if (strchr(CHARSET, str[i])) {
            short key_pos = strchr(CHARSET, key[pos_in_key]) - CHARSET;
            short char_pos = strchr(CHARSET, str[i]) - CHARSET;

            short char_new_pos = char_pos - key_pos;
            if (char_new_pos < 0) char_new_pos += strlen(CHARSET);
            temp[k++] = CHARSET[char_new_pos];
            pos_in_key = (pos_in_key + 1) % key_length;
        }
    }
    temp[k++] = '\0';
    char* decrypted_str = (char*) malloc(sizeof(char) * strlen(temp));
    memcpy(decrypted_str, temp, sizeof(char) * strlen(temp));
    return decrypted_str;
}

