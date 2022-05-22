#include "sha256.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

static const uint32_t K[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// ========================== UTILS ==========================

void print_header(const char* s) {
  printf("===================%s===================\n", s);
}

void print_binary(unsigned n, int bits)
{
    unsigned i;
    for (i = 1 << (bits - 1); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

uint64_t to_big_endian(uint64_t a) {
  uint64_t byte0 = ((a >> 0) & 0xFF)  << (8 * 7);
  uint64_t byte1 = ((a >> 8) & 0xFF)  << (8 * 6);
  uint64_t byte2 = ((a >> 16) & 0xFF) << (8 * 5);
  uint64_t byte3 = ((a >> 24) & 0xFF) << (8 * 4);
  uint64_t byte4 = ((a >> 32) & 0xFF) << (8 * 3);
  uint64_t byte5 = ((a >> 40) & 0xFF) << (8 * 2);
  uint64_t byte6 = ((a >> 48) & 0xFF) << (8 * 1);
  uint64_t byte7 = ((a >> 56) & 0xFF) << (8 * 0);

  return byte0 | byte1 | byte2 | byte3 | byte4 | byte5 | byte6 | byte7;
}

// ======================== BYTEARRAY ========================

bytearray* bytearray_create(char* s) {
  size_t len = strlen(s);
  uint8_t* data = (uint8_t*) malloc(sizeof(char) * len);
  memcpy(data, s, len);

  bytearray* ba = (bytearray*) malloc(sizeof(bytearray));
  ba->data = data;
  ba->len = len;
  return ba;
}


bytearray* bytearray_copy(bytearray* ba) {
  uint8_t* data_copy = (uint8_t*) malloc(sizeof(uint8_t) * ba->len);
  memcpy(data_copy, ba->data, ba->len);

  bytearray* copy = (bytearray*) malloc(sizeof(bytearray));
  copy->data = data_copy;
  copy->len = ba->len;

  return copy;
}


void bytearray_print(bytearray* ba) {
  for (uint64_t i = 0; i < ba->len; i++) {
    if (i % 8 == 0) printf("\n");
    printf("%02X ", ba->data[i]);
    // print_binary(ba->data[i], 8); printf(" ");
  }
  printf("\n");
}


void bytearray_delete(bytearray* ba) {
  free(ba->data);
  free(ba);
}

// ===========================================================
sha256_ctx* sha256_init() {
  sha256_ctx* ctx = malloc(sizeof(sha256_ctx));
  ctx->h[0] = 0x6a09e667;
  ctx->h[1] = 0xbb67ae85;
	ctx->h[2] = 0x3c6ef372;
	ctx->h[3] = 0xa54ff53a;
	ctx->h[4] = 0x510e527f;
	ctx->h[5] = 0x9b05688c;
	ctx->h[6] = 0x1f83d9ab;
	ctx->h[7] = 0x5be0cd19;
  return ctx;
}

bytearray* sha256_hash(bytearray* ba) {
  sha256_ctx* ctx = sha256_init();

  print_header("PADDING");
  printf("unpadded: ");
  bytearray_print(ba);
  sha256_pad(ba);
  printf("padded: "); bytearray_print(ba);
  printf("padded len: %lld\n", ba->len);

  sha256_loop(ba, ctx);
  bytearray* hash = malloc(sizeof(bytearray));

  hash->len = 32;
  hash->data = malloc(sizeof(uint8_t) * hash->len);
  for (int i = 0; i < 8; i++) {
    printf("h%d: %08llX ", i, ctx->h[i]);
    hash->data[i * 4 + 0] = ctx->h[i] >> (8 * 3);
    hash->data[i * 4 + 1] = ctx->h[i] >> (8 * 2);
    hash->data[i * 4 + 2] = ctx->h[i] >> (8 * 1);
    hash->data[i * 4 + 3] = ctx->h[i] >> (8 * 0);
  }
  printf("\n");

  return hash;
}


void sha256_pad(bytearray* ba) {
  uint64_t l = ba->len;
  uint64_t bytes_to_append = ((l + 1) % 56 == 0 ? 0 : (56 - (l + 1) % 56));
  uint64_t total_bytes_to_append = bytes_to_append + 1 + 8;
  uint64_t new_len = l + total_bytes_to_append;

  uint8_t* buf = malloc(sizeof(uint8_t) * (ba->len + total_bytes_to_append));
  buf[ba->len] = 1 << 7;
  for (int i = l + 1; i < new_len; i++) {
    buf[i] = 0;
  }
  uint64_t big_endian = to_big_endian(l * 8);
  for (int i = 0; i < 8; ++i) {
    buf[new_len - 8 + i] = (big_endian >> (8 * i)); 
  }

  memcpy(buf, ba->data, ba->len);
  free(ba->data);

  ba->data = buf;
  ba->len = new_len; 
}


void sha256_loop(bytearray* ba, sha256_ctx* ctx) {
  uint32_t w[64];
  for (int i = 0; i < ba->len; i += 64) {
    for (int j = 0; j < 16; j++) {
      int w_start = i + j * 4;
      w[j] = ((uint32_t)ba->data[w_start + 0]) << (8 * 3) 
           | ((uint32_t)ba->data[w_start + 1]) << (8 * 2)
           | ((uint32_t)ba->data[w_start + 2]) << (8 * 1)
           | ((uint32_t)ba->data[w_start + 3]) << (8 * 0);
    }

    for (int j = 16; j < 64; j++) {
      // uint32_t s0 = SIGMA0(w[j - 15]);
      // uint32_t s1 = SIGMA1(w[j - 2]);
      // printf("rotr7: "); print_binary(ROTR(w[j - 15], 7), 32); printf("\n");
      // printf("rotr18: "); print_binary(ROTR(w[j - 15], 18), 32); printf("\n");
      // printf("rshift3: "); print_binary(SHR(w[j - 15], 3), 32); printf("\n");
      // printf("s0: "); print_binary(s0, 32); printf("\n");
      // printf("s1: "); print_binary(s1, 32); printf("\n");
      w[j] = SIG1(w[j - 2]) + w[j - 7] + SIG0(w[j - 15]) + w[j - 16];
    }

    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];
    uint32_t e = ctx->h[4];
    uint32_t f = ctx->h[5];
    uint32_t g = ctx->h[6];
    uint32_t h = ctx->h[7];

    for (int j = 0; j < 64; j++) {
      uint32_t t1 = h + EP1(e) + CH(e, f, g) + K[i] + w[i];
      uint32_t t2 = EP0(a) + MAJ(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }
    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;
    // printf("a: "); print_binary(a, 32); printf("\n");
    // printf("b: "); print_binary(b, 32); printf("\n");
    // printf("c: "); print_binary(c, 32); printf("\n");
    // printf("d: "); print_binary(d, 32); printf("\n");
    // printf("e: "); print_binary(e, 32); printf("\n");
    // printf("f: "); print_binary(f, 32); printf("\n");
    // printf("g: "); print_binary(g, 32); printf("\n");
    // printf("h: "); print_binary(h, 32); printf("\n");
  }
}

