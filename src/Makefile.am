lib_LTLIBRARIES=libaesrand.la

libaesrand_la_LDFLAGS = -no-undefined -version-info 0:0:0
libaesrand_la_CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -fopenmp
libaesrand_la_LIBADD = -lm -lssl -lcrypto

libaesrand_la_SOURCES = aesrand.c aesrand_gmp.c aesrand_flint.c \
	aesrand_mpfr.c

include_HEADERS = aesrand.h aesrand_gmp.h aesrand_flint.h \
	aesrand_mpfr.h
