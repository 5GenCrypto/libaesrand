#include "aesrand_buf.h"

#include <assert.h>
#include <string.h>

unsigned char *
random_aes(aes_randstate_t state, size_t nbits, size_t *len)
{
    const size_t blocksize = EVP_CIPHER_CTX_block_size(state->ctx);
    const size_t nbytes =
        ((nbits / (blocksize * 8))
         + ((nbits % (blocksize * 8)) ? 1 : 0)) * blocksize;
    unsigned char *output;

    output = malloc(nbytes + EVP_MAX_IV_LENGTH);

    assert(EVP_CIPHER_iv_length(AES_ALGORITHM) != 0);

    {
        unsigned char in[nbytes];
        unsigned char iv[EVP_CIPHER_iv_length(AES_ALGORITHM)];
        int final_len = 0;
        size_t outlen = 0;

        memset(in, '\0', nbytes);
        memset(iv, '\0', EVP_CIPHER_iv_length(AES_ALGORITHM));

#pragma omp critical
        {
            memcpy(iv, &state->ctr, sizeof state->ctr);
            state->ctr++;
        }

        /* Compute E(K, CTR, 0 || 0 || ...) */
        EVP_EncryptInit_ex(state->ctx, NULL, NULL, state->key, iv);
        while (outlen < nbytes) {
            int buflen = 0;
            EVP_EncryptUpdate(state->ctx, output + outlen, &buflen, in, nbytes);
            outlen += buflen;
        }

        EVP_EncryptFinal_ex(state->ctx, output + outlen, &final_len);
        outlen += final_len;

        if (outlen > nbytes) {
            outlen = nbytes; // we will only use nbytes bytes
        }
    }

    *len = nbytes;
    return output;
}
