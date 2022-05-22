#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "sha256.h"

int main(void) {
  uint64_t key = 0x133457799BBCDFF1;

  char text[] = "hello world";
  bytearray* ba = bytearray_create(text);
  bytearray* hash = sha256_hash(ba);

  printf("hash:");
  bytearray_print(hash);

  bytearray_delete(hash);
  bytearray_delete(ba);
}

