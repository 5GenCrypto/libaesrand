#ifndef _AESRAND_MPFR_H_
#define _AESRAND_MPFR_H_

#include "aesrand.h"
#include <mpfr.h>

void
mpfr_urandomb_aes(mpfr_t rop, aes_randstate_t state);

#endif
