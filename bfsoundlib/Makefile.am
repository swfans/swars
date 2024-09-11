ACLOCAL_AMFLAGS = --install -I m4

lib_LIBRARIES = libbfsound.a

libbfsound_a_SOURCES = \
  include/ail.h \
  src/ail.c \
  include/aila.h \
  src/aila.c \
  include/aildebug.h \
  src/aildebug.c \
  include/ailss.h \
  src/ailss.c \
  src/ailvoc.c \
  src/ailwav.c \
  include/bfaudio.h \
  src/bfaudio.c \
  include/bfmusic.h \
  include/bfsound.h \
  include/bfscd.h \
  src/cd.c \
  include/dllload.h \
  src/dllload.c \
  include/dpmi.h \
  src/dpmi.c \
  include/drv_oal.h \
  src/drv_oal.c \
  src/init_mus.c \
  src/init_snd.c \
  include/memfile.h \
  src/memfile.c \
  include/miscutil.h \
  src/miscutil.c \
  src/mseqnfad.c \
  src/mseqnlst.c \
  src/mseqnply.c \
  include/mssdig.h \
  src/mssdig.c \
  include/msssys.h \
  src/msssys.c \
  include/mssxdig.h \
  src/mssxdig.c \
  include/mssxmidi.h \
  src/mssxmidi.c \
  include/oggvorbis.h \
  src/oggvorbis.c \
  include/sb16.h \
  src/sb16.c \
  include/awe32.h \
  src/awe32.c \
  src/awe32use.c \
  include/snderr.h \
  src/snderr.c \
  include/sndtimer.h \
  src/sndtimer.c \
  include/ssampfad.h \
  src/ssampfad.c \
  include/ssamplst.h \
  src/ssamplst.c \
  include/ssampply.h \
  src/ssampply.c \
  include/streamfx.h \
  src/streamfx.c \
  include/bfsvaribl.h \
  src/variable.c

libbfsound_a_CPPFLAGS = \
  -I"$(top_srcdir)/include" -I"$(builddir)/include"

include_HEADERS = $(wildcard $(srcdir)/include/*.h) $(builddir)/include/bsndconf.h
