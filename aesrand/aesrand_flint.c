#include "aesrand_flint.h"
#include "aesrand_gmp.h"

int
fmpz_mod_poly_randtest_aes(fmpz_mod_poly_t f, aes_randstate_t state, slong len)
{
    slong i;

    fmpz_mod_poly_fit_length(f, len);

    for (i = 0; i < len; i++) {
        if (fmpz_randm_aes(f->coeffs + i, state, &(f->p)) == AESRAND_ERR)
            return AESRAND_ERR;
    }

    _fmpz_mod_poly_set_length(f, len);
    _fmpz_mod_poly_normalise(f);
    return AESRAND_OK;
}

int
fmpz_randm_aes(fmpz_t f, aes_randstate_t state, const fmpz_t m)
{
    int ret;
    mpz_t x, rop;

    mpz_init(x);
    mpz_init(rop);
    fmpz_get_mpz(x, m);
    if ((ret = mpz_urandomm_aes(rop, state, x)) == AESRAND_OK) {
        fmpz_set_mpz(f, rop);
    }
    mpz_clear(x);
    mpz_clear(rop);
    return ret;
}

int
fmpz_randbits_aes(fmpz_t out, aes_randstate_t state, mp_bitcnt_t bits)
{
    int ret;
    mpz_t rop;
    mpz_init(rop);
    if ((ret = mpz_urandomb_aes(rop, state, bits)) == AESRAND_OK) {
        fmpz_set_mpz(out, rop);
    }
    mpz_clear(rop);
    return ret;
}


