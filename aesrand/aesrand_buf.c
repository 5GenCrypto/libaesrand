#include "aesrand_buf.h"

#include <assert.h>
#include <string.h>

unsigned char *
random_aes(aes_randstate_t state, size_t nbits, size_t *len)
{
    unsigned char iv[EVP_CIPHER_iv_length(AES_ALGORITHM)];
    EVP_CIPHER_CTX *ctx;
    unsigned char *in, *out;

    assert(EVP_CIPHER_iv_length(AES_ALGORITHM));
    memset(iv, '\0', EVP_CIPHER_iv_length(AES_ALGORITHM));
    ctx = EVP_CIPHER_CTX_new();
#pragma omp critical
    {
        memcpy(iv, &state->ctr, sizeof state->ctr);
        state->ctr++;
    }
    EVP_EncryptInit_ex(ctx, AES_ALGORITHM, NULL, state->key, iv);

    {
        const size_t blocksize = EVP_CIPHER_CTX_block_size(ctx);
        const size_t nbytes =
            ((nbits / (blocksize * 8))
             + ((nbits % (blocksize * 8)) ? 1 : 0)) * blocksize;
        int final_len = 0;
        size_t outlen = 0;

        out = malloc(nbytes + EVP_MAX_IV_LENGTH);
        /* nbytes may be huge, so we cannot use stack allocation for buf */
        in = calloc(nbytes, sizeof(unsigned char));

        /* Compute E(K, CTR, 0 || 0 || ...) */
        while (outlen < nbytes) {
            int buflen = 0;
            EVP_EncryptUpdate(ctx, out + outlen, &buflen, in, nbytes);
            outlen += buflen;
        }
        EVP_EncryptFinal_ex(ctx, out + outlen, &final_len);
        outlen += final_len;

        *len = nbytes;          /* only use nbytes bytes */
        free(in);
    }
    EVP_CIPHER_CTX_cleanup(ctx);

    return out;
}
