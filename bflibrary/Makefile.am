ACLOCAL_AMFLAGS = --install -I m4

lib_LIBRARIES = libbullfrog.a

libbullfrog_a_SOURCES = \
  src/general/arctan.c \
  src/general/box_clip.c \
  src/general/ganywnd.c \
  src/general/gbox.c \
  src/general/gbuffer.cpp \
  src/general/gcircle.c \
  src/general/gdata.c \
  src/general/gendian.c \
  src/general/gerrorlg.c \
  src/general/gexe_key.c \
  src/general/gfile.c \
  src/general/gfnuniq.c \
  src/general/gfont.c \
  src/general/ggenf.c \
  src/general/ggengh.c \
  src/general/giff.c \
  src/general/gini.c \
  src/general/gkeybd.c \
  src/general/gline.c \
  src/general/gmemut.c \
  src/general/gmouse.c \
  src/general/gpalcrss.c \
  src/general/gpalette.c \
  src/general/gplanar.c \
  src/general/gpng.c \
  src/general/gscreen.c \
  src/general/gsprite.c \
  src/general/gspr_col.c \
  src/general/gspr_map.c \
  src/general/gspr_std.c \
  src/general/gstrut.c \
  src/general/gtext.c \
  src/general/gtrig.c \
  src/general/gutility.c \
  src/general/hvline.c \
  src/general/mshandler.cpp \
  src/general/mspointer.cpp \
  src/general/pixel.c \
  src/general/poly.c \
  src/general/poly_trigp.c \
  src/general/poly_trigr.c \
  src/general/rnc_1fm.c \
  src/general/rom.c \
  src/general/scr_copy.c \
  src/general/spr_mcur.c \
  src/general/spr_scl.c \
  src/general/spr_scol.c \
  src/general/spr_smap.c \
  src/general/spr_sstd.c \
  src/general/spr_ssts.c \
  src/general/spr_ssta.c \
  src/general/sqroot.c

if USE_SDL2
libbullfrog_a_SOURCES += \
  src/x86-win-sdl2/sdir.c \
  src/x86-win-sdl2/sdrive.c \
  src/x86-win-sdl2/sffind.c \
  src/x86-win-sdl2/sfile.c \
  src/x86-win-sdl2/skeybd.cpp \
  src/x86-win-sdl2/slog.c \
  src/x86-win-sdl2/smemory.c \
  src/x86-win-sdl2/smouse.cpp \
  src/x86-win-sdl2/spalette.c \
  src/x86-win-sdl2/sscreen.c \
  src/x86-win-sdl2/sscrsurf.c \
  src/x86-win-sdl2/ssemphr.cpp \
  src/x86-win-sdl2/stime.c \
  src/x86-win-sdl2/svesa.c \
  src/x86-win-sdl2/swindows.c
else
libbullfrog_a_SOURCES += \
  src/x86-win-sdl/sdir.c \
  src/x86-win-sdl/sdrive.c \
  src/x86-win-sdl/sffind.c \
  src/x86-win-sdl/sfile.c \
  src/x86-win-sdl/skeybd.cpp \
  src/x86-win-sdl/slog.c \
  src/x86-win-sdl/smemory.c \
  src/x86-win-sdl/smouse.cpp \
  src/x86-win-sdl/spalette.c \
  src/x86-win-sdl/sscreen.c \
  src/x86-win-sdl/sscrsurf.c \
  src/x86-win-sdl/ssemphr.cpp \
  src/x86-win-sdl/stime.c \
  src/x86-win-sdl/svesa.c \
  src/x86-win-sdl/swindows.c
endif

libbullfrog_a_CPPFLAGS = \
  -I$(top_srcdir)/include -I$(builddir)/include

include_HEADERS = $(wildcard include/*.h) $(wildcard include/*.hpp)

check_PROGRAMS = bflib_test_math bflib_test_memory bflib_test_poly

bflib_test_math_SOURCES = \
  tests/bflib_test_math.c

bflib_test_math_CPPFLAGS = \
  -I$(top_srcdir)/include -I$(builddir)/include

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_bflib_test_math_SOURCES = dummy.cxx

bflib_test_math_LDADD = \
  -L$(builddir) -lbullfrog

bflib_test_memory_SOURCES = \
  tests/mock_windows.c \
  tests/bflib_test_memory.c

bflib_test_memory_CPPFLAGS = \
  -I$(top_srcdir)/include -I$(builddir)/include

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_bflib_test_memory_SOURCES = dummy.cxx

bflib_test_memory_LDADD = \
  -L$(builddir) -lbullfrog

bflib_test_poly_SOURCES = \
  tests/mock_mouse.c \
  tests/mock_palette.c \
  tests/mock_screen.c \
  tests/mock_windows.c \
  tests/helpers_screen.c \
  tests/bflib_test_poly.c

bflib_test_poly_CPPFLAGS = \
  -I$(top_srcdir)/include -I$(builddir)/include

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_bflib_test_poly_SOURCES = dummy.cxx

bflib_test_poly_LDADD = \
  -L$(builddir) -lbullfrog

TESTS = $(check_PROGRAMS)
