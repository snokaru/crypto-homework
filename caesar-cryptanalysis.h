#ifndef CAESAR_CRYPTANALYSIS_H
#define CAESAR_CRYPTANALYSIS_H

#include <string.h>
#include <stdlib.h>

#define CHARSET "abcdefghijklmnopqrstuvwxyz"

char** caesar_generate_all_permutations(char* str, int* len);

#endif
