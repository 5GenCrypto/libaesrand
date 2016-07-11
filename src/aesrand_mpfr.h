#ifndef _AESRAND_MPFR_H_
#define _AESRAND_MPFR_H_

#include "aesrand.h"
#include <mpfr.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mpfr_urandomb_aes(mpfr_t rop, aes_randstate_t state);

#ifdef __cplusplus
}
#endif

#endif
