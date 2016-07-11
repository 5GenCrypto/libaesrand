#include "aesrand_gmp.h"

void
mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t n)
{
#pragma omp critical
    {
        const mp_bitcnt_t nb = n/8+1; // number of bytes
        unsigned char *output;
        mp_bitcnt_t outlen = 0;
        int final_len = 0;

        // update the internal counter, works at most 2^64 times
        memcpy(state->iv, &state->ctr, sizeof(state->ctr));

        state->ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(state->ctx, AES_ALGORITHM, NULL, state->key, state->iv);

        output = malloc(2 * (nb + EVP_MAX_IV_LENGTH));

        {
            unsigned char *in;
            const int in_size = nb;

            in = malloc(in_size);
            memset(in, 0, in_size);

            while (outlen < nb) {
                int buflen = 0;
                EVP_EncryptUpdate(state->ctx, output+outlen, &buflen, in, in_size);
                state->ctr++;
                outlen += buflen;
            }
            EVP_EncryptFinal(state->ctx, output+outlen, &final_len);
            outlen += final_len;

            if (outlen > nb) {
                outlen = nb; // we will only use nb bytes
            }
            free(in);
        }

        {
            unsigned char *buf;
            mp_bitcnt_t true_len = outlen + 4;
            mp_bitcnt_t bytelen = outlen;
            FILE *fp;

            buf = malloc(true_len);
            memset(buf, 0, true_len);
            memcpy(buf+4, output, outlen);
            buf[4] >>= ((outlen*8) - (unsigned int) n);

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

        EVP_CIPHER_CTX_cleanup(state->ctx);
        free(state->ctx);
    }
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
