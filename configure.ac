AC_INIT([libaesrand], [0.1.0], [bcarmer@gmail.com])

AC_CONFIG_HEADERS([aesrand/config.h])

AC_CONFIG_SRCDIR([aesrand])
AC_CONFIG_AUX_DIR([build/autoconf])
AC_CONFIG_MACRO_DIR([build/autoconf])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AM_PROG_AR

LT_INIT

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_ARG_ENABLE(debug,
[AS_HELP_STRING([--enable-debug], [enable debugging])],
[], [enable_debug=no])

AC_ARG_ENABLE(flint,
[AS_HELP_STRING([--disable-flint], [do not compile flint support])],
[], [enable_flint=yes])

AC_ARG_ENABLE(mpfr,
[AS_HELP_STRING([--disable-mpfr], [do not compile mpfr support])],
[], [enable_mpfr=yes])

CFLAGS=                         dnl get rid of default -g -O2
COMMON_CFLAGS="-Wall -Wformat -Wformat-security -Wextra -Wunused \
-Wshadow -Wmissing-prototypes -Wfloat-equal -Wpointer-arith -Wcast-align \
-Wstrict-prototypes -Wredundant-decls -Wendif-labels -Wcast-qual \
-std=gnu11 -march=native -Wpedantic"

if test "x$enable_debug" = x"yes"; then
  EXTRA_CFLAGS="-O0 -g"
else
  EXTRA_CFLAGS="-fomit-frame-pointer -O3"
  AC_DEFINE(NDEBUG,1,[Define whether debugging is enabled])
fi
AC_SUBST(COMMON_CFLAGS)
AC_SUBST(EXTRA_CFLAGS)

AC_CHECK_HEADERS([omp.h])

AC_SEARCH_LIBS(__gmpz_init, gmp, [], AC_MSG_ERROR([libgmp not found]))
if test "x$enable_flint" = x"yes"; then
  AC_SEARCH_LIBS(_fmpz_clear_mpz, flint, [], AC_MSG_ERROR([libflint not found]))
  AC_DEFINE(AESRAND_HAVE_FLINT,1,[Define whether flint is enabled])
  AC_SUBST(AESRAND_FLINT_INCLUDE, ["#include \"aesrand/aesrand_flint.h\""])
else
  AC_SUBST(AESRAND_FLINT_INCLUDE, [""])
fi
if test "x$enable_mpfr" = x"yes"; then
  AC_SEARCH_LIBS(mpfr_init2, mpfr, [], AC_MSG_ERROR([libmpfr not found]))
  AC_DEFINE(AESRAND_HAVE_MPFR,1,[Define whether mpfr is enabled])
  AC_SUBST(AESRAND_MPFR_INCLUDE, ["#include \"aesrand/aesrand_mpfr.h\""])
else
AC_SUBST(AESRAND_MPFR_INCLUDE, [""])
fi

AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile aesrand/Makefile test/Makefile aesrand/aesrand.h])

AC_OUTPUT
