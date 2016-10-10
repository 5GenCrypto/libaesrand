#ifndef _AESRAND_H_
#define _AESRAND_H_

#define _GNU_SOURCE

#ifdef __cplusplus
extern "C" {
#endif

#include "aesrand/aesrand_config.h"

#include "aesrand/aesrand_init.h"
#include "aesrand/aesrand_buf.h"
#include "aesrand/aesrand_gmp.h"
#ifdef AESRAND_HAVE_FLINT
  #include "aesrand/aesrand_flint.h"
#endif
#ifdef AESRAND_HAVE_MPRF
  #include "aesrand/aesrand_mpfr.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* _AESRAND_H_ */
