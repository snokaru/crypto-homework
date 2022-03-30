#include <stdio.h>
#include <stdlib.h>
#include "polybius.h"
#include "caesar-cryptanalysis.h"
#include "hill.h"
#include "vigenere.h"
#include "vigenere-cryptanalysis.h"


void test_vigenere() {
    char* original = "this is an example of the vigenere cipher";
    char* key = "vector";
    printf("original: %s\n", original);

    char* encrypted = vigenere_encrypt(original, key);
    printf("encrypted: %s\n", encrypted);

    char* decrypted = vigenere_decrypt(original, key);
    printf("decrpyed: %s\n", decrypted);
}

void test_vigenere_cryptanalysis() {
    char* original = "vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmumshwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluysdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt";
    int len = 0;
    printf("original: %s\n", original);
    char** keys = vigenere_generate_most_likely_keys(original, &len);
    for (int i = 0; i < len; ++i) {
        printf("possible key: %s\n", keys[i]);
        char* possible_decryption = vigenere_decrypt(original, keys[i]);
        printf("possible decryption: %s\n", possible_decryption);
        free(possible_decryption);
    }
}

int main(void) {
    printf("Running vigenere test...\n");
    test_vigenere();
    printf("\nRunning vigenere cryptanalysis test...\n");
    test_vigenere_cryptanalysis();
}

