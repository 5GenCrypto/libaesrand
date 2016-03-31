AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = '-Ibuild/autoconf'

AM_CFLAGS = ${DEBUG_CFLAGS} -I$(top_srcdir) -I$(top_srcdir)/dgs -fopenmp -std=c99

lib_LTLIBRARIES=libaesrand.la

MY_LIB_LDFLAGS=-release 0.1.0 -no-undefined

libaesrand_la_SOURCES = aesrand.c
libaesrand_la_LDFLAGS = $(MY_LIB_LDFLAGS)
libaesrand_la_INCLUDEDOR = $(includedir)/aesrand
libaesrand_la_LIBADD = -lflint -lgmp -lmpfr -lm -lssl -lcrypto

include_HEADERS = aesrand.h
