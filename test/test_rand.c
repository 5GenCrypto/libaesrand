#include "aesrand.h"

#include <sys/time.h>

static double
current_time(void)
{
    struct timeval t;
    (void) gettimeofday(&t, NULL);
    return (double) (t.tv_sec + (double) (t.tv_usec / 1000000.0));
}

static void
print_buf(unsigned char *buf, size_t size)
{
    printf("0x");
    for (size_t i = 0; i < size; ++i) {
        printf("%02x", buf[i]);
    }
    printf("\n");
}

#define NITERS 100000

int
main(void)
{
    aes_randstate_t rng;
    unsigned char *buf;
    size_t nbits, nbytes;
    double start, end;

    if (aes_randinit(rng) == 1)
        return 1;

    nbits = 128;

    start = current_time();
    for (int i = 0; i < NITERS; ++i) {
        buf = random_aes(rng, nbits, &nbytes);
        free(buf);
    }
    end = current_time();
    printf("Time: %f s\n", end - start);

    start = current_time();
    {
        mpz_t rop;
        mpz_init(rop);
        for (int i = 0; i < NITERS; ++i) {
            if (mpz_urandomb_aes(rop, rng, 128) == AESRAND_ERR)
                return 1;
        }
        mpz_clear(rop);
    }
    end = current_time();
    printf("Time: %f s\n", end - start);
    
    return 0;
}
