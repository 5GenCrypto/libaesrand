#ifndef _AESRAND_BUF_H_
#define _AESRAND_BUF_H_

#include "aesrand_init.h"

/* Takes a state and a number of *bits*, and returns a buffer with 'nbits / 8 +
 * 1' bytes of randomness amenable for use by mpz_urandomb_aes.  'len' contains
 * the number of *bytes* contained in the resulting buffer.
 */
unsigned char*
random_aes(aes_randstate_t state, size_t nbits, size_t *len);

#endif
