#include "aesrand_flint.h"
#include "aesrand_gmp.h"

void fmpz_mod_poly_randtest_aes(fmpz_mod_poly_t f, aes_randstate_t state,
    slong len) {
    slong i;

    fmpz_mod_poly_fit_length(f, len);

    for (i = 0; i < len; i++) {
      fmpz_randm_aes(f->coeffs + i, state, &(f->p));
    }

    _fmpz_mod_poly_set_length(f, len);
    _fmpz_mod_poly_normalise(f);
}

void fmpz_randm_aes(fmpz_t f, aes_randstate_t state, const fmpz_t m) {
  mpz_t x, rop;
  mpz_init(x);
  mpz_init(rop);
  fmpz_get_mpz(x, m);
  mpz_urandomm_aes(rop, state, x);
  fmpz_set_mpz(f, rop);
  mpz_clear(x);
  mpz_clear(rop);
}

void fmpz_randbits_aes(fmpz_t out, aes_randstate_t state, mp_bitcnt_t bits) {
  mpz_t rop;
  mpz_init(rop);
  mpz_urandomb_aes(rop, state, bits);
  fmpz_set_mpz(out, rop);
  mpz_clear(rop);
}


