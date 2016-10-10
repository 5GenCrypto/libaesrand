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
    size_t nbytes;

    buf = random_aes(state, n, &nbytes);

    /* Convert to format amenable by mpz_inp_raw */
    {
        const size_t true_len = nbytes + 4;
        size_t bytelen = nbytes;

        buf = calloc(true_len, sizeof(unsigned char));
        memcpy(buf + 4, buf, nbytes);
        buf[4] >>= ((nbytes * 8) - (unsigned int) n);

        for (int i = 3; i >= 0; i--) {
            buf[i] = (unsigned char) (bytelen % (1 << 8));
            bytelen /= (1 << 8);
        }
    }

    fp = fmemopen(buf, nbytes, "rb");
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
