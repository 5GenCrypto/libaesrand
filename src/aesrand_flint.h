#ifndef _AESRAND_FLINT_H_
#define _AESRAND_FLINT_H_

#include "aesrand.h"

#include <flint/fmpz_poly.h>
#include <flint/fmpz_mod_poly.h>

#ifdef __cplusplus
extern "C" {
#endif

void fmpz_mod_poly_randtest_aes(fmpz_mod_poly_t f, aes_randstate_t state,
    slong len);
void fmpz_randm_aes(fmpz_t f, aes_randstate_t state, const fmpz_t m);
void fmpz_randbits_aes(fmpz_t out, aes_randstate_t state, const mp_bitcnt_t bits);

#ifdef __cplusplus
}
#endif

#endif
