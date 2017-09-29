cmake_minimum_required(VERSION 3.0)

project(aesrand
  VERSION 1.0.0
  DESCRIPTION "library for random number generation using AES"
)
include(GNUInstallDirs)
add_library(aesrand SHARED
  src/aesrand_buf.c
  src/aesrand_flint.c
  src/aesrand_gmp.c
  src/aesrand_init.c
  src/aesrand_mpfr.c
)
set_target_properties(aesrand PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
)

target_link_libraries(aesrand PRIVATE gmp)

# find_library(LIBOMP omp)
# target_link_libraries(aesrand PRIVATE "${LIBOMP}")

option(HAVE_FLINT "Define whether flint is enabled" OFF)
option(HAVE_MPFR  "Define whether mpfr is enabled"  OFF)

configure_file(aesrand.pc.in aesrand.pc @ONLY)
target_include_directories(aesrand PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
install(TARGETS aesrand
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES ${CMAKE_BINARY_DIR/aesrand.pc}
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

if (HAVE_FLINT)
  target_link_libraries(aesrand PRIVATE flint)
endif (HAVE_FLINT)
if (HAVE_MPFR)
  target_link_libraries(aesrand PRIVATE mpfr)
endif (HAVE_MPFR)
