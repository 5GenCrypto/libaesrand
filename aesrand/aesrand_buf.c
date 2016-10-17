#include "aesrand_buf.h"

#include <string.h>
#include <openssl/evp.h>

unsigned char *
random_aes(aes_randstate_t state, size_t nbits, size_t *len)
{
    const size_t nbytes = nbits / 8 + 1;
    unsigned char *output;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();
    output = malloc(nbytes + EVP_MAX_IV_LENGTH);

    {
        unsigned char in[nbytes];
        unsigned char iv[EVP_CIPHER_iv_length(AES_ALGORITHM)];
        int final_len = 0;
        size_t outlen = 0;

        memset(in, '\0', nbytes);
        memset(iv, '\0', EVP_CIPHER_iv_length(AES_ALGORITHM));

#pragma omp critical
        {
            memcpy(iv, &state->ctr, sizeof(state->ctr));
            state->ctr++;
        }

        /* Compute E(K, CTR, 0 || 0 || ...) */
        EVP_EncryptInit_ex(ctx, AES_ALGORITHM, NULL, state->key, iv);
        while (outlen < nbytes) {
            int buflen = 0;
            EVP_EncryptUpdate(ctx, output + outlen, &buflen, in, nbytes);
            outlen += buflen;
        }

        EVP_EncryptFinal(ctx, output + outlen, &final_len);
        outlen += final_len;

        if (outlen > nbytes) {
            outlen = nbytes; // we will only use nbytes bytes
        }
    }

    EVP_CIPHER_CTX_cleanup(ctx);
    free(ctx);

    *len = nbytes;
    return output;
}
