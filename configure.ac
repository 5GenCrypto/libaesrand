AC_INIT([libaesrand], [0.1.0], [bcarmer@gmail.com])

AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_AUX_DIR([build/autoconf])
AC_CONFIG_MACRO_DIR([build/autoconf])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AM_PROG_AR

LT_INIT

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_ARG_ENABLE(debug,        [  --enable-debug          Enable assert() statements for debugging.], [enable_debug=yes])

CFLAGS=                         dnl get rid of default -g -O2
COMMON_CFLAGS="-Wall -Wformat -Wformat-security -Wextra -Wunused \
-Wshadow -Wmissing-prototypes -Wfloat-equal -Wpointer-arith -Wcast-align \
-Wstrict-prototypes -Wredundant-decls -Wendif-labels -Wcast-qual \
-std=gnu11 -Wpedantic"

if test "x$enable_debug" = x"yes"; then
  EXTRA_CFLAGS="-O0 -g"
else
  EXTRA_CFLAGS="-O3"
  AC_DEFINE(NDEBUG,1,[Define whether debugging is enabled])
fi
AC_SUBST(COMMON_CFLAGS)
AC_SUBST(EXTRA_CFLAGS)

AC_CHECK_HEADERS([omp.h])

AC_SEARCH_LIBS(__gmpz_init,gmp)
if test "x$ac_cv_search___gmpz_init" = "xno"; then
  AC_MSG_ERROR([libgmp not found])
fi
AC_SEARCH_LIBS(_fmpz_clear_mpz,flint)
if test "x$ac_cv_search__fmpz_clear_mpz" = "xno"; then
  AC_MSG_ERROR([libflint not found])
fi
AC_SEARCH_LIBS(mpfr_init2,mpfr)
if test "x$ac_cv_search_mpfr_init2" = "xno"; then
  AC_MSG_ERROR([libmpfr not found])
fi

AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
