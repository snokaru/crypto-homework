#include "hill.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHARSET "abcdefghijklmnopqrstuvwxyz"

int** _key_to_matrix(char* key, int n) {
    if (strlen(key) < n * n) return 0;
    int** matrix = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * n);
    }

    char* current_char = key;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; ++j) {
            char* pos = strchr(CHARSET, *current_char++);
            matrix[i][j] = pos - CHARSET;
        }
    }
    return matrix;
}

int _determinant(int** matrix, int n) {
    if (n == 2) 
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    return 0;
}

void _matrix_to_adjugate(int** matrix, int n) {
    if (n == 2) {
        int aux = matrix[0][0];
        matrix[0][0] = matrix[1][1];
        matrix[1][1] = aux;
        matrix[0][1] = -matrix[0][1];
        matrix[1][0] = -matrix[1][0];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0 ; j < n; j++) {
            if (matrix[i][j] < 0)
                matrix[i][j] += strlen(CHARSET);
            else 
                matrix[i][j] = matrix[i][j] % strlen(CHARSET);
        }
    } 
}

// this can be faster
int _multiplicative_inverse_modulo(int n, int modulo) {
    for (int i = 1; i < modulo; i++) {
        if (((n % modulo) * (i % modulo)) % modulo == 1) {
            return i;
        }
    }
    return -1;
}

void _matrix_invert(int** matrix, int n) {
    int det = _determinant(matrix, n);
    if (det < 0)
        det += strlen(CHARSET);
    else
        det %= strlen(CHARSET);

    int mult_inverse = _multiplicative_inverse_modulo(det, strlen(CHARSET));
    _matrix_to_adjugate(matrix, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (matrix[i][j] * mult_inverse) % strlen(CHARSET);
        }
    }
}

void _multiply_matrix_vector(int** matrix, int* vector, int n) {
    int temp[n];

    for (int i = 0; i < n; i++) {
        temp[i] = 0;
        for (int j = 0; j < n; j++) {
            temp[i] += matrix[i][j] * vector[j];
        }
    }

    for (int i = 0; i < n; i++) {
        vector[i] = temp[i];
    }
}

char* _obtain_code(char* str, int** matrix, int n) {
    char* str_copy = (char*)malloc(sizeof(char) * strlen(str));
    strcpy(str_copy, str);

    // Remove any characters that are not in CHARSET
    for (int i = 0; str_copy[i]; ++i) {
        if (!strchr(CHARSET, str_copy[i])) {
            memmove(str_copy + i, str_copy + i + 1, strlen(str_copy));
            i--;
        }
    }

    char* result = (char*)malloc(sizeof(char) * n);
    int* vector = (int*)malloc(sizeof(int) * n);
    int current = 0;
    for (int i = 0; str_copy[i]; i += n) {
        for (int j = i; j < i + n; j++) {
            vector[j - i] = strchr(CHARSET, str_copy[j]) - CHARSET; 
        }
        _multiply_matrix_vector(matrix, vector, n);

        for (int i = 0; i < n; i++) {
            vector[i] = vector[i] % strlen(CHARSET);
            result[current++] = CHARSET[vector[i]];
        }
    }
    free(vector);
    free(str_copy);

    result[current++] = '\0';
    return result;
}

char* hill_encode(char* str, char* key, int n) {
    int** matrix = _key_to_matrix(key, n);
    if (!matrix) return 0;
    char* encoded = _obtain_code(str, matrix, n);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return encoded;

}

char* hill_decode(char* str, char* key, int n) {
    int** matrix = _key_to_matrix(key, n);
    if (!matrix) return 0;
    _matrix_invert(matrix, n);

    char* decoded = _obtain_code(str, matrix, n);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return decoded;
}

