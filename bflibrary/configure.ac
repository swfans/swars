#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])

m4_define([bflibrary_version], [0.1.2.0])

AC_INIT([bullfrog-library], [bflibrary_version], [mefistotelis@gmail.com], [bflibrary])

AC_CONFIG_SRCDIR([src/general/gfile.c])
AC_CONFIG_HEADERS([include/bfconfig.h])
#AC_CONFIG_LINKS([include:include])
# custom command for AC_CONFIG_LINKS([referenc/:tests/referenc/*.png])
AC_CONFIG_COMMANDS([tests_referenc], [
	mkdir -p "$ac_builddir/referenc"
	cp -f "$ac_srcdir/tests/referenc/"*.png "$ac_builddir/referenc"
])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Define arguments.

AC_ARG_WITH([sdl2],
    AS_HELP_STRING([--without-sdl2], [Build without use of SDL2 library]))

AC_ARG_ENABLE([bflib-wscreen-control],
    AS_HELP_STRING([--disable-bflib-wscreen-control],
        [disallow the bflibrary to change value of lbDisplay.WScreen]),
    [bflib_wscreen_control=$enableval], [bflib_wscreen_control=no])

AS_IF([test "$bflib_wscreen_control" = "yes"], [
    AC_DEFINE([BFLIB_WSCREEN_CONTROL], 1,
        [Define to 1 if the bflibrary should change value of lbDisplay.WScreen])
])

# Checks for programs.

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.

AS_IF([test "x$with_sdl2" != "xno"],
  [AM_PATH_SDL2([2.0.0], [
    have_sdl2=yes], [
    have_sdl2=no])], [
  have_sdl2=no
])
AC_MSG_CHECKING([whether to use SDL2 library])
AS_IF([test "x$have_sdl2" == "xyes"], [
  AC_DEFINE(BFLIB_ENABLE_SDL2, [1], [Define for SDL2 support])
], [
  AS_IF([test "x$with_sdl2" = "xyes"], [
    AC_MSG_ERROR([sdl2 requested but not found])
  ])
])
AC_MSG_RESULT([$have_sdl2])

AS_IF([test "x$have_sdl2" != "xyes"], [
  AM_PATH_SDL([1.2.0], [], [
    AC_MSG_ERROR([could not find SDL2 nor SDL1 library])
  ])
  AC_DEFINE(BFLIB_ENABLE_SDL1, [1], [Define for SDL1 support])
])

PKG_CHECK_MODULES([PNG], [libpng], [], [
  AC_MSG_ERROR([libpng not found])
])

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

# Flags

CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

CFLAGS="$CFLAGS $PNG_CFLAGS"
LIBS="$LIBS $PNG_LIBS"

# Final commands

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
