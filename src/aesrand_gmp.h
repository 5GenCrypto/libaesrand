#ifndef _AESRAND_GMP_H_
#define _AESRAND_GMP_H_

#include "aesrand_init.h"
#include <gmp.h>

#ifdef __cplusplus
extern "C" {
#endif

void mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t n);
void mpz_urandomm_aes(mpz_t rop, aes_randstate_t state, const mpz_t n);

#ifdef __cplusplus
}
#endif

#endif
