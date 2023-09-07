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

langallger_WADFILES = \
    ger/alltext.wad \
    ger/alltext.idx

langallger_POFILES = \
	alltext_syndct_ger.po \
	alltext_church_ger.po \
	alltext_punks_ger.po \
	alltext_comm_ger.po

langallgerdir = $(prefix)/opt/$(PACKAGE)/language/ger
langallger_DATA = $(langallger_WADFILES)

$(langallger_WADFILES): $(langallger_POFILES)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallita_WADFILES = \
    ita/alltext.wad \
    ita/alltext.idx

langallita_POFILES = \
	alltext_syndct_ita.po \
	alltext_church_ita.po \
	alltext_punks_ita.po \
	alltext_comm_ita.po

langallitadir = $(prefix)/opt/$(PACKAGE)/language/ita
langallita_DATA = $(langallita_WADFILES)

$(langallita_WADFILES): $(langallita_POFILES)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

EXTRA_DIST = \
	langallfre \
	langallger \
	langallita \
	langalleng
