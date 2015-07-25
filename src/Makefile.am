bin_PROGRAMS = swars

EXTRA_DIST = Makefile.windows windows.config.h

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	display.c \
	display.h \
	dos.c \
	dos.h \
	game.c \
	game.h \
	keyboard.c \
	keyboard.h \
	main.c \
	memory.c \
	mouse.c \
	mouse.h \
	oggvorbis.c \
	oggvorbis.h \
	sound.c \
	sound.h \
	sound_util.c \
	sound_util.h \
	swars.S \
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
