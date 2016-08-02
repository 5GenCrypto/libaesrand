#include "aesrand.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef RANDFILE
#  define RANDFILE "/dev/urandom"
#endif

int
aes_randinit(aes_randstate_t rng)
{
    int file;
    if ((file = open(RANDFILE, O_RDONLY)) == -1) {
        fprintf(stderr, "Error opening %s\n", RANDFILE);
        return 1;
    } else {
        unsigned char seed[16];
        if (read(file, seed, 16) == -1) {
            fprintf(stderr, "Error reading from %s\n", RANDFILE);
            close(file);
            return 1;
        } else {
            aes_randinit_seedn(rng, (char *) seed, 16, NULL, 0);
        }
    }
    if (file != -1)
        close(file);
    return 0;
}

void
aes_randinit_seedn(aes_randstate_t state, char *seed, size_t seed_len,
                   char *additional, size_t additional_len)
{
    SHA256_CTX sha256;

    state->aes_init = 1;
    state->ctr = 0;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, seed, seed_len);
    SHA256_Update(&sha256, additional, additional_len);
    SHA256_Final(state->key, &sha256);

    printf("seed: ");
    for (size_t i = 0; i < seed_len; ++i) {
        printf("0x%02x, ", (unsigned char) seed[i]);
    }
    printf("\n");
}

void
aes_randclear(aes_randstate_t state __attribute__ ((unused)))
{
}
