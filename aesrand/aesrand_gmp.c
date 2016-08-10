#include "aesrand_gmp.h"
#include "aesrand_buf.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>

void
mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t n)
{
    unsigned char *buf;
    FILE *fp;

    buf = random_aes(state, &n);
    fp = fmemopen(buf, n, "rb");
    if (!fp) {
        printf("Error in generating randomness.\n");
    }
    if (mpz_inp_raw(rop, fp) == 0) {
        printf("Error in parsing randomness.\n");
    }
    fclose(fp);
    free(buf);
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
