cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(aesrand VERSION 1.0.0 LANGUAGES C)

include(GNUInstallDirs)

option(HAVE_FLINT "Define whether flint is enabled" ON)
option(HAVE_MPFR  "Define whether mpfr is enabled"  ON)
set(AESRAND_HAVE_FLINT ${HAVE_FLINT})
set(AESRAND_HAVE_MPFR  ${HAVE_MPFR})

set(aesrand_SOURCES
  aesrand/aesrand_buf.c
  aesrand/aesrand_gmp.c
  aesrand/aesrand_init.c
  )
set(aesrand_HEADERS
  aesrand/aesrand.h
  aesrand/aesrand_init.h
  aesrand/aesrand_buf.h
  aesrand/aesrand_gmp.h
  )

if (AESRAND_HAVE_FLINT)
  list(APPEND aesrand_SOURCES aesrand/aesrand_flint.c)
  list(APPEND aesrand_HEADERS aesrand/aesrand_flint.h)
endif (AESRAND_HAVE_FLINT)
if (AESRAND_HAVE_MPFR)
  list(APPEND aesrand_SOURCES aesrand/aesrand_mpfr.c)
  list(APPEND aesrand_HEADERS aesrand/aesrand_mpfr.h)
endif (AESRAND_HAVE_MPFR)

add_library(aesrand SHARED ${aesrand_SOURCES})
target_include_directories(aesrand INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/aesrand>
  $<INSTALL_INTERFACE:include/aesrand>)


target_link_libraries(aesrand PRIVATE crypto gmp)
if (AESRAND_HAVE_FLINT)
  target_link_libraries(aesrand PRIVATE flint)
endif (AESRAND_HAVE_FLINT)
if (AESRAND_HAVE_MPFR)
  target_link_libraries(aesrand PRIVATE mpfr)
endif (AESRAND_HAVE_MPFR)

include(FindOpenMP)
target_compile_options(aesrand
  PRIVATE
  -Wall -Wextra -Wpedantic -Werror -std=gnu11 -march=native ${OpenMP_C_FLAGS})
target_link_libraries(aesrand INTERFACE "${OpenMP_C_FLAGS}")

configure_file(
  "${PROJECT_SOURCE_DIR}/aesrand/aesrand.h.in" "${PROJECT_SOURCE_DIR}/aesrand/aesrand.h"
  )

install(TARGETS aesrand
  LIBRARY DESTINATION lib
  )
install(FILES ${aesrand_HEADERS}
  DESTINATION include/aesrand)

# Test files

add_executable(test_aesrand test/test_aesrand.c)
target_include_directories(test_aesrand PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_aesrand PRIVATE aesrand gmp)
if (AESRAND_HAVE_FLINT)
  target_link_libraries(test_aesrand PRIVATE flint)
endif (AESRAND_HAVE_FLINT)
if (AESRAND_HAVE_MPFR)
  target_link_libraries(test_aesrand PRIVATE mpfr)
endif (AESRAND_HAVE_MPFR)

enable_testing()
add_test(NAME test_aesrand COMMAND test_aesrand)
