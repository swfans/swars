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
  src/general/gexe_key.cpp \
  src/general/gfile.c \
  src/general/gfnuniq.c \
  src/general/gfont.c \
  src/general/ggenf.c \
  src/general/ggengh.c \
  src/general/giff.c \
  src/general/gkeybd.c \
  src/general/gline.c \
  src/general/gmemut.c \
  src/general/gmouse.c \
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
  src/general/gutility.cpp \
  src/general/hvline.c \
  src/general/mshandler.cpp \
  src/general/mspointer.cpp \
  src/general/pixel.c \
  src/general/poly.c \
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

libbullfrog_a_CPPFLAGS = \
  -I$(top_srcdir)/include -I$(builddir)/include

include_HEADERS = $(wildcard include/*.h) $(wildcard include/*.hpp)
