AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = '-Ibuild/autoconf'

AM_CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -fopenmp

lib_LTLIBRARIES=libaesrand.la

libaesrand_la_SOURCES = aesrand.c
libaesrand_la_LDFLAGS = -no-undefined -version-info 0:0:0
libaesrand_la_LIBADD = -lm -lssl -lcrypto

include_HEADERS = aesrand.h
