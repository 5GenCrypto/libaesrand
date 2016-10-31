AM_CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -I$(top_srcdir)/aesrand -I$(top_srcdir)
AM_LDFLAGS = $(top_builddir)/aesrand/libaesrand.la

check_PROGRAMS = test_rand

TESTS = $(check_PROGRAMS)

test_rand_SOURCES = test_rand.c

all: $(TESTS)
