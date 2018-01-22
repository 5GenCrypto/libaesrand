cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(aesrand VERSION 1.0.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

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
if (AESRAND_HAVE_FLINT)
  list(APPEND aesrand_SOURCES aesrand/aesrand_flint.c)
endif (AESRAND_HAVE_FLINT)
if (AESRAND_HAVE_MPFR)
  list(APPEND aesrand_SOURCES aesrand/aesrand_mpfr.c)
endif (AESRAND_HAVE_MPFR)

add_library(aesrand SHARED ${aesrand_SOURCES})
target_link_libraries(aesrand PUBLIC crypto gmp)
if (AESRAND_HAVE_FLINT)
  target_link_libraries(aesrand PUBLIC flint)
endif (AESRAND_HAVE_FLINT)
if (AESRAND_HAVE_MPFR)
  target_link_libraries(aesrand PUBLIC mpfr)
endif (AESRAND_HAVE_MPFR)

find_package(OpenMP REQUIRED)
target_link_libraries(aesrand INTERFACE "${OpenMP_C_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-unused-result -std=gnu11 -march=native ${OpenMP_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg -ggdb -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

configure_file(
  "${PROJECT_SOURCE_DIR}/aesrand/aesrand.h.in" "${PROJECT_SOURCE_DIR}/aesrand/aesrand.h"
  )

install(TARGETS aesrand LIBRARY DESTINATION lib)
install(FILES aesrand/aesrand.h DESTINATION include)

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
