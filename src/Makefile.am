bin_PROGRAMS = swars

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	ariadne.sx \
	bflib_basics.h \
	bflib_fmvids.c \
	bflib_fmvids.h \
	bflib_fmvids_s.sx \
	bflib_joyst.c \
	bflib_joyst.h \
	bflib_joyst_s.sx \
	bflib_render.c \
	bflib_render_gpoly.sx \
	bflib_render.h \
	bflib_render_plin.sx \
	bflib_render_trig_s.sx \
	bflib_snd_cda.sx \
	bflib_sndlib.sx \
	bflib_snd_mss.sx \
	bflib_snd_sys.sx \
	bflib_vidraw.sx \
	config.h \
	display.c \
	display.h \
	dos.c \
	dos.h \
	game.c \
	game_data.c \
	game_data.h \
	game.h \
	globals.h \
	keyboard.c \
	keyboard.h \
	main.c \
	mouse.c \
	mouse.h \
	oggvorbis.c \
	oggvorbis.h \
	sound.c \
	sound.h \
	sound_util.c \
	sound_util.h \
	swars.sx \
	thing_debug.sx \
	timer.c \
	timer.h \
	unix.c \
	unix.h \
	util.c \
	util.h \
	windows.c \
	windows.h

swars_CPPFLAGS = \
  -I"$(top_srcdir)/bflibrary/include"

swars_WRAPPERS = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

swars_RCFLAGS = \
  -I"$(top_srcdir)/src" -I"$(builddir)" \
  $(RCFLAGS)

if HAS_WINDRES
swars_RESRCS = swars_stdres.res
else
swars_RESRCS =
endif

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_swars_SOURCES = dummy.cxx

swars_LDADD = \
  $(swars_WRAPPERS) $(swars_RESRCS) \
  -L$(builddir)/../bflibrary -lbullfrog

$(swars_WRAPPERS:.o=.sx): %.sx: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.sx.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

if HAS_WINDRES
%.res: $(top_srcdir)/res/%.rc
	$(WINDRES) $(swars_RCFLAGS) -i $< -J rc -o $@ -O coff
endif

CLEANFILES = wrappers_*.sx
DISTCLEANFILES = *~
