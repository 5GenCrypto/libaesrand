#include "aesrand.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef RANDFILE
#  define RANDFILE "/dev/urandom"
#endif

int
aes_randinit(aes_randstate_t rng)
{
    int ret = AESRAND_ERR;
    int file;

    if ((file = open(RANDFILE, O_RDONLY)) == -1) {
        fprintf(stderr, "Error opening %s\n", RANDFILE);
        return AESRAND_ERR;
    } else {
        unsigned char seed[16];
        if (read(file, seed, 16) == -1) {
            fprintf(stderr, "Error reading from %s\n", RANDFILE);
            goto cleanup;
        }
        aes_randinit_seedn(rng, (char *) seed, 16, NULL, 0);
    }
    ret = AESRAND_OK;
cleanup:
    if (file != -1)
        close(file);
    return ret;
}

void
aes_randinit_seedn(aes_randstate_t state, char *seed, size_t seed_len,
                   char *additional, size_t additional_len)
{
    SHA256_CTX sha256;
    unsigned char key[SHA256_DIGEST_LENGTH];

    state->ctr = 0;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, seed, seed_len);
    if (additional)
        SHA256_Update(&sha256, additional, additional_len);
    SHA256_Final(key, &sha256);
    memcpy(state->key, key, sizeof state->key);
}

void
aes_randclear(aes_randstate_t state)
{
    (void) state;
}

int
aes_randstate_fwrite(const aes_randstate_t state, FILE *fp)
{
    (void) fwrite(&state->ctr, sizeof state->ctr, 1, fp);
    (void) fwrite(state->key, sizeof state->key, 1, fp);
    return AESRAND_OK;
}

int
aes_randstate_fread(aes_randstate_t state, FILE *fp)
{
    (void) fread(&state->ctr, sizeof state->ctr, 1, fp);
    (void) fread(state->key, sizeof state->key, 1, fp);
    return AESRAND_OK;
}

int
aes_randstate_write(const aes_randstate_t state, const char *fname)
{
    FILE *f;
    if ((f = fopen(fname, "w")) == NULL) {
        perror(fname);
        return AESRAND_ERR;
    }
    (void) aes_randstate_fwrite(state, f);
    fclose(f);
    return AESRAND_OK;
}

int
aes_randstate_read(aes_randstate_t state, const char *fname)
{
    FILE *f;
    if ((f = fopen(fname, "r")) == NULL) {
        perror(fname);
        return AESRAND_ERR;
    }
    (void) aes_randstate_fread(state, f);
    fclose(f);
    return AESRAND_OK;
}
