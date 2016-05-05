AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = '-Ibuild/autoconf'

AM_CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -fopenmp

lib_LTLIBRARIES=libaesrand.la

libaesrand_la_SOURCES = aesrand.c
libaesrand_la_LDFLAGS = -release 0.1.0 -no-undefined
libaesrand_la_LIBADD = -lm -lssl -lcrypto

include_HEADERS = aesrand.h
