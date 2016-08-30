#ifndef _AESRAND_BUF_H_
#define _AESRAND_BUF_H_

#include "aesrand_init.h"

/* Takes a state and a number of *bits*, and returns a buffer with randomness
 * amenable for use by mpz_urandomb_aes.  'n' is rewritten to contain the number
 * of *bytes* contained in the buffer.
 *
 * Note that if you want to use this for randomness, the first 5 bytes are extra
 * bookkeeping information, and should NOT be used!
 */
unsigned char*
random_aes(aes_randstate_t state, size_t *n);

#endif
