#ifndef _AESRAND_BUF_H_
#define _AESRAND_BUF_H_

#include "aesrand_init.h"

unsigned char*
random_aes(aes_randstate_t state, size_t *n);

#endif
