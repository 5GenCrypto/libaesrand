#include "aesrand_gmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>

void
mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t n)
{
    const mp_bitcnt_t nb = n/8+1; // number of bytes
    unsigned char *output;
    mp_bitcnt_t outlen = 0;
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
        unsigned char *buf;
        const mp_bitcnt_t true_len = outlen + 4;
        mp_bitcnt_t bytelen = outlen;
        FILE *fp;

        buf = malloc(true_len);
        memset(buf, 0, true_len);
        memcpy(buf + 4, output, outlen);
        buf[4] >>= ((outlen * 8) - (unsigned int) n);

        for (int i = 3; i >= 0; i--) {
            buf[i] = (unsigned char) (bytelen % (1 << 8));
            bytelen /= (1 << 8);
        }

        // this generates a random n-bit number.
        fp = fmemopen(buf, true_len, "rb");
        if (!fp) {
            printf("Error in generating randomness.\n");
        }

        if (mpz_inp_raw(rop, fp) == 0) {
            printf("Error in parsing randomness.\n");
        }

        fclose(fp);
        free(buf);
    }

    free(output);

    EVP_CIPHER_CTX_cleanup(ctx);
    free(ctx);
}

void
mpz_urandomm_aes(mpz_t rop, aes_randstate_t state, const mpz_t n)
{
    unsigned long size = mpz_sizeinbase(n, 2);

    while (1) {
        mpz_urandomb_aes(rop, state, size);
        if (mpz_cmp(rop, n) < 0) {
            break;
        }
    }
}
