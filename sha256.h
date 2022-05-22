#ifndef _SHA256_H
#define _SHA256_H

#include <inttypes.h>

typedef struct {
  uint8_t* data;
  uint64_t len;
} bytearray;

typedef struct {
  uint64_t h[8];
} sha256_ctx;


bytearray* bytearray_copy(bytearray* ba); 
bytearray* bytearray_create(char* s);
void       bytearray_delete(bytearray* ba);
void       bytearray_print(bytearray* ba);

bytearray* sha256_hash(bytearray* ba);
void       sha256_pad(bytearray* ba);
void       sha256_loop(bytearray* ba, sha256_ctx* ctx);

#endif
