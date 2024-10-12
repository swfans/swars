#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])

m4_define([bfsoundlib_version], [0.1.0.0])

AC_INIT([bullfrog-sound-library], [bfsoundlib_version], [mefistotelis@gmail.com], [bfsoundlib])

AC_CONFIG_SRCDIR([src/bfaudio.c])
AC_CONFIG_HEADERS([include/bsndprivconf.h])
AX_PREFIX_CONFIG_H([include/bsndconf.h], [LBS])
#AC_CONFIG_LINKS([include:include])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Define arguments.

AC_ARG_WITH([wildmidi],
    AS_HELP_STRING([--without-wildmidi], [Build without use of WildMIDI library]))

# Checks for programs.

AC_PROG_CC
AC_PROG_CXX
PKG_PROG_PKG_CONFIG
AC_PROG_RANLIB

# Checks for libraries.

PKG_CHECK_MODULES([VORBISFILE], [vorbisfile >= 1.2.0], [], [
  AC_MSG_ERROR([vorbisfile >= 1.2.0 not found])
])

AM_PATH_OPENAL([], [
  AC_MSG_ERROR([OpenAL not found])
])

AS_IF([test "x$with_wildmidi" != "xno"],
  [AC_CHECK_LIB([WildMidi], [WildMidi_Init], [
    have_wildmidi=yes], [
    have_wildmidi=no])], [
  have_wildmidi=no
])
AC_MSG_CHECKING([whether to use WildMIDI library])
AS_IF([test "x$have_wildmidi" == "xyes"], [
  AC_DEFINE(ENABLE_WILDMIDI, [1], [Define for wildmidi library support])
  AC_SUBST(WILDMIDI_LIBS, -lWildMidi)
], [
  AC_SUBST(WILDMIDI_LIBS, )
  AS_IF([test "x$with_wildmidi" = "xyes"], [
    AC_MSG_ERROR([wildmidi library requested but not found])
  ])
])
AC_SUBST(WILDMIDI_CFLAGS, )
AC_MSG_RESULT([$have_wildmidi])

# libraries from within this project are only built after `make` is executed
AC_SUBST(BFLIBRARY_LIBS, "-L\"\$(top_builddir)/../lib\" -lbullfrog")
AC_SUBST(BFLIBRARY_CFLAGS, "-I\"\$(top_builddir)/../include/bullfrog\"")


# Checks for header files.
AC_CHECK_HEADERS([stdint.h unistd.h])

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
AC_CHECK_FUNCS([memmove memset])

# Flags

CFLAGS="$CFLAGS -Wall -Wextra -Wno-unused-parameter"
CFLAGS="$CFLAGS -include \"\$(top_builddir)/include/bsndconf.h\""

CFLAGS="$CFLAGS $OPENAL_CFLAGS"
LIBS="$LIBS $OPENAL_LIBS"

CFLAGS="$CFLAGS $VORBISFILE_CFLAGS"
LIBS="$LIBS $VORBISFILE_LIBS"

CFLAGS="$CFLAGS $WILDMIDI_CFLAGS"
LIBS="$LIBS $WILDMIDI_LIBS"

CFLAGS="$CFLAGS $BFLIBRARY_CFLAGS"
LIBS="$LIBS $BFLIBRARY_LIBS"

# Final commands

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
