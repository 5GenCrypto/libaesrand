#ifndef _AESRAND_GMP_H_
#define _AESRAND_GMP_H_

#include "aesrand_init.h"
#include <gmp.h>

int mpz_urandomb_aes(mpz_t rop, aes_randstate_t state, mp_bitcnt_t n);
int mpz_urandomm_aes(mpz_t rop, aes_randstate_t state, const mpz_t n);

#endif
