TEXTWAD = $(PYTHON) $(top_srcdir)/util/textwad.py

langalleng_WADFILES = \
    eng/alltext.wad \
    eng/alltext.idx

langalleng_POFILES = \
	alltext_syndct_eng.pot \
	alltext_church_eng.pot \
	alltext_punks_eng.pot \
	alltext_comm_eng.pot

langallengdir = $(prefix)/opt/$(PACKAGE)/language/eng
langalleng_DATA = $(langalleng_WADFILES)

$(langalleng_WADFILES): $(langalleng_POFILES)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(top_srcdir)/conf/char_encoding_tbl_euwad.txt" -l "eng" -w $@ -c $+

EXTRA_DIST = \
	langalleng
