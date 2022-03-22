#include <stdio.h>
#include <stdlib.h>
#include "polybius.h"
#include "caesar-cryptanalysis.h"
#include "hill.h"

void test_caesar() {
    char *test = "the quick brown fox jumps over the lazy dog";
    int *encoded, len;
    char** permutations = caesar_generate_all_permutations(test, &len);
    for (int i = 0; i < len; ++i) {
        printf("%s\n", permutations[i]);
    }

    for (int i = 0; i < len; ++i) {
        free(permutations[i]);
    }
    free(permutations);
}

void test_polybius() {
    int len;
    printf("original: %s\n", "short example");
    int* encoded = polybius_encode("short example", &len);
    printf("encoded: ");
    for (int i = 0; i < len; ++i) {
        printf("%d ", encoded[i]);
    }
    printf("\n");

    char* decoded = polybius_decode(encoded, len);
    printf("decoded: %s\n", decoded);
    free(encoded);
    free(decoded);
}

void test_hill() {
    printf("original: %s\n", "short example");
    char* encoded = hill_encode("short example", "hill", 2);
    printf("encoded: %s\n", encoded);
    char* decoded = hill_decode(encoded, "hill", 2);
    printf("decoded: %s\n", decoded);
    free(encoded);
    free(decoded);
}

int main(void) {
    printf("polybius\n");
    test_polybius();
    printf("\nhill\n");
    test_hill();
    printf("\ncaesar\n");
    test_caesar();
}

