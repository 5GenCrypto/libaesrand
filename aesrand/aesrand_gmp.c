#include "aesrand.h"

#include <string.h>

int
mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t nbits)
{
    unsigned char *rand;
    FILE *fp;
    size_t nbytes;
    int ret = AESRAND_ERR;

    rand = random_aes(state, nbits, &nbytes);

    /* Convert to format amenable by mpz_inp_raw */
    {
        const size_t true_len = nbytes + 4;
        unsigned char *buf;
        /* nbytes may be huge, so we cannot use stack allocation for buf */
        buf = calloc(true_len, sizeof(unsigned char));
        memcpy(buf + 4, rand, nbytes);
        buf[4] >>= ((nbytes * 8) - (unsigned int) nbits);

        for (int i = 3; i >= 0; i--) {
            buf[i] = (unsigned char) (nbytes % (1 << 8));
            nbytes /= (1 << 8);
        }

        if ((fp = fmemopen(buf, true_len, "rb")) == NULL)
            goto cleanup;
        /* setbuf(fp, NULL);       /\* Avoid buffering to speed things up *\/ */
        if (mpz_inp_raw(rop, fp) == 0)
            goto cleanup;
        ret = AESRAND_OK;
    cleanup:
        free(buf);
        if (fp)
            fclose(fp);
    }
    free(rand);
    return ret;
}

int
mpz_urandomm_aes(mpz_t rop, aes_randstate_t state, const mpz_t n)
{
    const unsigned long size = mpz_sizeinbase(n, 2);

    while (1) {
        if (mpz_urandomb_aes(rop, state, size) == AESRAND_ERR)
            return AESRAND_ERR;
        if (mpz_cmp(rop, n) < 0)
            return AESRAND_OK;
    }
}
