#include "aesrand_buf.h"

#include <string.h>
#include <openssl/evp.h>

unsigned char *
random_aes(aes_randstate_t state, size_t *n)
{
    const size_t nb = *n / 8 + 1;
    unsigned char *output, *buf;
    size_t outlen = 0;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();
    output = malloc(2 * (nb + EVP_MAX_IV_LENGTH));

    {
        unsigned char *in;
        unsigned char *iv;
        const int in_size = nb;
        int final_len = 0;

        in = malloc(in_size);
        memset(in, 0, in_size);

        iv = malloc(EVP_CIPHER_iv_length(AES_ALGORITHM));
        memset(iv, 0, EVP_CIPHER_iv_length(AES_ALGORITHM));

#pragma omp critical
        {
            memcpy(iv, &state->ctr, sizeof(state->ctr));
            state->ctr++;
        }

        EVP_EncryptInit_ex(ctx, AES_ALGORITHM, NULL, state->key, iv);
        while (outlen < nb) {
            int buflen = 0;
            EVP_EncryptUpdate(ctx, output + outlen, &buflen, in, in_size);
            outlen += buflen;
        }

        EVP_EncryptFinal(ctx, output + outlen, &final_len);
        outlen += final_len;

        if (outlen > nb) {
            outlen = nb; // we will only use nb bytes
        }
        free(in);
        free(iv);
    }

    {
        const size_t true_len = outlen + 4;
        size_t bytelen = outlen;

        buf = malloc(true_len);
        memset(buf, 0, true_len);
        memcpy(buf + 4, output, outlen);
        buf[4] >>= ((outlen * 8) - (unsigned int) *n);

        for (int i = 3; i >= 0; i--) {
            buf[i] = (unsigned char) (bytelen % (1 << 8));
            bytelen /= (1 << 8);
        }

        free(output);
        *n = true_len;
    }

    EVP_CIPHER_CTX_cleanup(ctx);
    free(ctx);

    return buf;
}
