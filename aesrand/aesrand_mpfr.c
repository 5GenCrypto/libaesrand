#include "aesrand.h"

int
mpfr_urandomb_aes(mpfr_t rop, aes_randstate_t state)
{
    const unsigned long size = mpfr_get_prec(rop);
    int ret;
    mpfr_t num, denom;
    mpz_t mpz_num, mpz_denom;
    mpz_init(mpz_num);
    mpz_init(mpz_denom);

    if ((ret = mpz_urandomb_aes(mpz_num, state, size)) == AESRAND_OK) {
        mpfr_init_set_z(num, mpz_num, MPFR_RNDN);
        mpz_ui_pow_ui(mpz_denom, 2, size);
        mpfr_init_set_z(denom, mpz_denom, MPFR_RNDN);
        mpfr_div(rop, num, denom, MPFR_RNDN);
    }
    mpz_clear(mpz_num);
    mpz_clear(mpz_denom);
    mpfr_clear(num);
    mpfr_clear(denom);
    return ret;
}
