#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.71])

m4_define([bflibrary_version], [0.1.0.0])

AC_INIT([bullfrog-library], [bflibrary_version], [mefistotelis@gmail.com], [bflibrary])
AC_CONFIG_SRCDIR([src/general/gfile.c])
AC_CONFIG_HEADERS([include/bfconfig.h])
#AC_CONFIG_LINKS([include:include])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd memmove memset mkdir rmdir])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
