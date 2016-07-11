#ifndef _AESRAND_H_
#define _AESRAND_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _aes_randstate_struct {
  char aes_init;
  unsigned long ctr;
  unsigned char key[SHA256_DIGEST_LENGTH];
  unsigned char *iv;
};

#define AES_ALGORITHM EVP_aes_256_ctr()

typedef struct _aes_randstate_struct aes_randstate_t[1];

int  aes_randinit(aes_randstate_t state); // seed from /dev/urandom
void aes_randinit_seed(aes_randstate_t state, char *seed, char *additional);
void aes_randinit_seedn(aes_randstate_t state, char *seed, size_t seed_size, char *additional, size_t additional_size);
void aes_randclear(aes_randstate_t state);

#ifdef __cplusplus
}
#endif

#endif /* _AESRAND_H_ */
