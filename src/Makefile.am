bin_PROGRAMS = swars

EXTRA_DIST = Makefile.windows windows.config.h

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	ariadne.s \
	bflib_dernc_s.s \
	bflib_fileio.c \
	bflib_fileio_s.s \
	bflib_fmvids.s \
	bflib_joyst_s.s \
    bflib_joyst.c \
	bflib_keybrd.s \
	bflib_math.s \
	bflib_memory.c \
	bflib_picture.c \
	bflib_render_gpoly.s \
	bflib_render_plin.s \
	bflib_render_trig.s \
	bflib_snd_cda.s \
	bflib_snd_mss.s \
	bflib_snd_sys.s \
	bflib_sndlib.s \
	bflib_video.s \
	bflib_vidraw.s \
	bflib_vidraw_spr_norm.s \
	display.c \
	display.h \
	dos.c \
	dos.h \
	game_data.c \
	game_data.h \
	game.c \
	game.h \
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
	swars.S \
	thing_debug.s \
	timer.c \
	timer.h \
	unix.c \
	unix.h \
	util.c \
	util.h \
	windows.c \
	windows.h

swars_LDADD = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

$(swars_LDADD:.o=.S): %.S: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.S.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

CLEANFILES = wrappers_*.S
DISTCLEANFILES = *~
