#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CHARSET "abcdefghijklmnopqrstuvwxyz"

const double freq[] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074
};

int _count_letters(char* str, char letter, int period) {
    int count = 0;
    for (int i = 0; i < strlen(str); i += period) {
        if (str[i] == letter) count++;
    }
    return count;
}

// Only used for debugging
void _print_with_period(char* str_to_print, int period) {
    for (int i = 0; i < strlen(str_to_print); i += period) {
        printf("%c ", str_to_print[i]);
    }
    printf("\n");
}

int _compute_period_length(char* str, int period) {
    int count = 0;
    for (int i = 0; i < strlen(str); i += period) {
        count++;
    }
    return count;
}

double _compute_average_IC(char* str, int key_length) {
    double average_IC = 0;
    for (int period = 1; period <= key_length; period++) {
        double IC = 0;
        double length = _compute_period_length(str+(period-1), key_length);
        for (int j = 0; j < strlen(CHARSET); j++) {
            double frequency = _count_letters(str + (period - 1), CHARSET[j], key_length);
            IC += (frequency * (frequency - 1)) / (length * (length - 1));
        }
        average_IC += IC;
    }
    return average_IC / (double) key_length;
}

double compute_chi(char* str, int start, int period, int caesar_shift) {
    char* str_copy = (char*) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(str_copy, str);

    // Perform caesar shift for each character separated by period.
    for (int i = start; i < strlen(str_copy); i += period) {
        int pos_in_charset = strchr(CHARSET, str_copy[i]) - CHARSET;
        int new_pos = pos_in_charset - caesar_shift;
        if (new_pos < 0) new_pos += strlen(CHARSET);
        str_copy[i] = CHARSET[new_pos];
    }

    // Compute CHI
    double chi = 0;
    for (int j = 0; j < strlen(CHARSET); j++) {
        double frequency = freq[j];  
        double count = _count_letters(str_copy + start, CHARSET[j], period); 
        double length = _compute_period_length(str_copy + start, period);
        chi += ((count - length * frequency) * (count - length * frequency)) / (length * frequency);
    }
    free(str_copy);
    return chi;
}

char* find_key_with_length(char* str, int len) {
    char tmp[len + 1];
    for (int i = 0; i < len; i++) {
        double min_chi = 0;
        for (int j = 0; j < strlen(CHARSET); j++) {
            double chi = compute_chi(str, i, len, j);
            if (min_chi == 0 || chi < min_chi) {
                min_chi = chi;
                tmp[i] = CHARSET[j];
            }
        }
    }
    tmp[len] = '\0';
    char* key = (char*)malloc(sizeof(char) * (len + 1));
    strcpy(key, tmp);
    return key;
}

char** vigenere_generate_most_likely_keys(char* str, int* len) {
    const int MAX_KEY_LENGTH = 20;
    double average_ics[MAX_KEY_LENGTH + 1];

    char* tmp[500];
    int current_key = 0;

    for (int i = 1; i <= MAX_KEY_LENGTH; i++) {
        double average_IC = _compute_average_IC(str, i);
        average_ics[i] = average_IC;
    }

    for (int i = 1; i <= MAX_KEY_LENGTH; i++) {
        if (average_ics[i] > 0.090) {
            char* best_key = find_key_with_length(str, i);
            tmp[current_key++] = best_key;
        }
    }

    char** keys = (char**)malloc(sizeof(char*) * current_key);
    for (int i = 0; i < current_key; ++i) {
        keys[i] = tmp[i];
    }
    *len = current_key;
    return keys;
}
