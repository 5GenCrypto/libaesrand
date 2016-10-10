lib_LTLIBRARIES=libaesrand.la

libaesrand_la_LDFLAGS = -no-undefined -version-info 0:0:0
libaesrand_la_CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -fopenmp -I$(top_builddir)
libaesrand_la_LIBADD = -lm -lssl -lcrypto

libaesrand_la_SOURCES = aesrand_init.c aesrand_buf.c aesrand_gmp.c	\
	aesrand_flint.c aesrand_mpfr.c

include_HEADERS = aesrand.h
pkgincludesubdir = $(includedir)/aesrand
pkgincludesub_HEADERS = aesrand_init.h aesrand_buf.h aesrand_gmp.h	\
	aesrand_flint.h aesrand_mpfr.h
