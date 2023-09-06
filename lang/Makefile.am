TEXTWAD = $(PYTHON) $(top_srcdir)/util/textwad.py

langall_eu_ETBLS = \
    $(top_srcdir)/conf/char_encoding_tbl_eu.txt \
    $(top_srcdir)/conf/char_encoding_tblm_eu.txt

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
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallfre_WADFILES = \
    fre/alltext.wad \
    fre/alltext.idx

langallfre_POFILES = \
	alltext_syndct_fre.po \
	alltext_church_fre.po \
	alltext_punks_fre.po \
	alltext_comm_fre.po

langallfredir = $(prefix)/opt/$(PACKAGE)/language/fre
langallfre_DATA = $(langallfre_WADFILES)

$(langallfre_WADFILES): $(langallfre_POFILES)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

EXTRA_DIST = \
	langallfre \
	langalleng
