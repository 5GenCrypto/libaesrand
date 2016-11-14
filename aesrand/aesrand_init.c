#include "aesrand_init.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef RANDFILE
#  define RANDFILE "/dev/urandom"
#endif

static inline void
initialize_ctx(aes_randstate_t state)
{
    state->ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(state->ctx, AES_ALGORITHM, NULL, state->key, NULL);
}

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
            return AESRAND_ERR;
        }
        aes_randinit_seedn(rng, (char *) seed, 16, NULL, 0);
    }
    if (file != -1)
        close(file);
    return AESRAND_OK;
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

    initialize_ctx(state);
}

void
aes_randclear(aes_randstate_t state)
{
    EVP_CIPHER_CTX_cleanup(state->ctx);
    free(state->ctx);
}

int
aes_randstate_fwrite(aes_randstate_t state, FILE *fp)
{
    fwrite(&state->aes_init, sizeof(state->aes_init), 1, fp);
    fwrite(&state->ctr, sizeof(state->ctr), 1, fp);
    fwrite(state->key, sizeof(state->key), 1, fp);
    return AESRAND_OK;
}

int
aes_randstate_fread(aes_randstate_t state, FILE *fp)
{
    fread(&state->aes_init, sizeof(state->aes_init), 1, fp);
    fread(&state->ctr, sizeof(state->ctr), 1, fp);
    fread(state->key, sizeof(state->key), 1, fp);
    initialize_ctx(state);
    return AESRAND_OK;
}

int
aes_randstate_write(aes_randstate_t state, const char *fname)
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
