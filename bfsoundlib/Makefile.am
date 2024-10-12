ACLOCAL_AMFLAGS = --install -I m4

lib_LIBRARIES = libbfsound.a

libbfsound_a_headers_src = \
  include/ail.h \
  include/aila.h \
  include/aildebug.h \
  include/ailss.h \
  include/bfaudio.h \
  include/bfdanger.h \
  include/bfmusic.h \
  include/bfsound.h \
  include/bfscd.h \
  include/dllload.h \
  include/dpmi.h \
  include/drv_oal.h \
  include/memfile.h \
  include/miscutil.h \
  include/mssal.h \
  include/mssdig.h \
  include/msssys.h \
  include/mssxdig.h \
  include/mssxmidi.h \
  include/oggvorbis.h \
  include/sb16.h \
  include/awe32.h \
  include/snderr.h \
  include/sndtimer.h \
  include/ssampfad.h \
  include/ssamplst.h \
  include/ssampply.h \
  include/streamfx.h \
  include/bfsvaribl.h

libbfsound_a_headers_bld = \
  include/bsndconf.h

libbfsound_a_SOURCES = \
  $(libbfsound_a_headers_src) \
  src/ail.c \
  src/aila.c \
  src/aildebug.c \
  src/ailss.c \
  src/ailvoc.c \
  src/ailwav.c \
  src/bfaudio.c \
  src/cd.c \
  src/dllload.c \
  src/dpmi.c \
  src/drv_oal.c \
  src/init_mus.c \
  src/init_snd.c \
  src/memfile.c \
  src/miscutil.c \
  src/mseqnfad.c \
  src/mseqnlst.c \
  src/mseqnply.c \
  src/mssdig.c \
  src/msssys.c \
  src/mssxdig.c \
  src/mssxmidi.c \
  src/oggvorbis.c \
  src/sb16.c \
  src/awe32.c \
  src/awe32use.c \
  src/snderr.c \
  src/sndtimer.c \
  src/ssampfad.c \
  src/ssamplst.c \
  src/ssampply.c \
  src/streamfx.c \
  src/variable.c

libbfsound_a_CPPFLAGS = \
  -I"$(top_srcdir)/include" -I"$(builddir)/include"

libbfsoundheadersdir = $(includedir)/bfsound
libbfsoundheaders_HEADERS = \
  $(libbfsound_a_headers_src:%=$(srcdir)/%) \
  $(libbfsound_a_headers_bld:%=$(builddir)/%)

# Libraries included with the project
libbfsound_a_PROJECT_LIBS = ../lib/libbullfrog.a

$(libbfsound_a_OBJECTS) $(lib_LIBRARIES): $(libbfsound_a_PROJECT_LIBS)

../lib/libbullfrog.a: ../bflibrary/libbullfrog.a
	$(MAKE) -C $(<D) DESTDIR="../" prefix="" install
