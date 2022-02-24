bin_PROGRAMS = swars

EXTRA_DIST = Makefile.windows windows.config.h

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	ariadne.S \
	bflib_basics.c \
	bflib_basics.h \
	bflib_dernc.c \
	bflib_dernc.h \
	bflib_fileio.c \
	bflib_fileio.h \
	bflib_filelst.c \
	bflib_filelst.h \
	bflib_fmvids.S \
	bflib_joyst.c \
	bflib_joyst.h \
	bflib_joyst_s.S \
	bflib_keybrd.h \
	bflib_keybrd_s.S \
	bflib_math.S \
	bflib_memory.c \
	bflib_memory.h \
	bflib_mouse.c \
	bflib_mouse.h \
	bflib_picture.c \
	bflib_picture.h \
	bflib_render.c \
	bflib_render_gpoly.S \
	bflib_render.h \
	bflib_render_plin.S \
	bflib_render_trig_s.S \
	bflib_snd_cda.S \
	bflib_sndlib.S \
	bflib_snd_mss.S \
	bflib_snd_sys.S \
	bflib_sprite.c \
	bflib_sprite.h \
	bflib_video.c \
	bflib_video.h \
	bflib_video_s.S \
	bflib_vidraw.S \
	bflib_vidraw_spr_norm.S \
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
	swars.S \
	thing_debug.S \
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
