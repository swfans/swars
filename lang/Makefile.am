TEXTDAT = $(PYTHON) $(top_srcdir)/util/textdat.py
TEXTWAD = $(PYTHON) $(top_srcdir)/util/textwad.py

langall_eu_ETBLS = \
    $(top_srcdir)/conf/char_encoding_tbl_eu.txt \
    $(top_srcdir)/conf/char_encoding_tblm_eu.txt

langalleng_DATFILES_C = \
    eng/guitext.dat

langalleng_WADFILES_S = \
    eng/alltext-s.wad \
    eng/alltext-s.idx

langalleng_WADFILES_Z = \
    eng/alltext-z.wad \
    eng/alltext-z.idx

langalleng_WADFILES_P = \
    eng/alltext-p.wad \
    eng/alltext-p.idx

langalleng_POFILES_C = \
    guitext_comm_eng.pot

langalleng_POFILES_S = \
	alltext_syndct_eng.pot \
	alltext_comm_eng.pot

langalleng_POFILES_Z = \
	alltext_church_eng.pot \
	alltext_comm_eng.pot

langalleng_POFILES_P = \
	alltext_punks_eng.pot \
	alltext_comm_eng.pot

langallengdir = $(prefix)/$(PACKAGE)/language/eng
langalleng_DATA = $(langalleng_DATFILES_C) $(langalleng_WADFILES_S) $(langalleng_WADFILES_Z) $(langalleng_WADFILES_P)

$(langalleng_DATFILES_C): $(langalleng_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 2,$(langall_eu_ETBLS))" -d $@ -c $+

$(langalleng_WADFILES_S): $(langalleng_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langalleng_WADFILES_Z): $(langalleng_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langalleng_WADFILES_P): $(langalleng_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallfre_DATFILES_C = \
    fre/guitext.dat

langallfre_WADFILES_S = \
    fre/alltext-s.wad \
    fre/alltext-s.idx

langallfre_WADFILES_Z = \
    fre/alltext-z.wad \
    fre/alltext-z.idx

langallfre_WADFILES_P = \
    fre/alltext-p.wad \
    fre/alltext-p.idx

langallfre_POFILES_C = \
    guitext_comm_fre.po

langallfre_POFILES_S = \
	alltext_syndct_fre.po \
	alltext_comm_fre.po

langallfre_POFILES_Z = \
	alltext_church_fre.po \
	alltext_comm_fre.po

langallfre_POFILES_P = \
	alltext_punks_fre.po \
	alltext_comm_fre.po

langallfredir = $(prefix)/$(PACKAGE)/language/fre
langallfre_DATA = $(langallfre_DATFILES_C) $(langallfre_WADFILES_S) $(langallfre_WADFILES_Z) $(langallfre_WADFILES_P)

$(langallfre_DATFILES_C): $(langallfre_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 2,$(langall_eu_ETBLS))" -d $@ -c $+

$(langallfre_WADFILES_S): $(langallfre_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallfre_WADFILES_Z): $(langallfre_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallfre_WADFILES_P): $(langallfre_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallger_DATFILES_C = \
    ger/guitext.dat

langallger_WADFILES_S = \
    ger/alltext-s.wad \
    ger/alltext-s.idx

langallger_WADFILES_Z = \
    ger/alltext-z.wad \
    ger/alltext-z.idx

langallger_WADFILES_P = \
    ger/alltext-p.wad \
    ger/alltext-p.idx

langallger_POFILES_C = \
    guitext_comm_ger.po

langallger_POFILES_S = \
	alltext_syndct_ger.po \
	alltext_comm_ger.po

langallger_POFILES_Z = \
	alltext_church_ger.po \
	alltext_comm_ger.po

langallger_POFILES_P = \
	alltext_punks_ger.po \
	alltext_comm_ger.po

langallgerdir = $(prefix)/$(PACKAGE)/language/ger
langallger_DATA = $(langallger_DATFILES_C) $(langallger_WADFILES_S) $(langallger_WADFILES_Z) $(langallger_WADFILES_P)

$(langallger_DATFILES_C): $(langallger_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 2,$(langall_eu_ETBLS))" -d $@ -c $+

$(langallger_WADFILES_S): $(langallger_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallger_WADFILES_Z): $(langallger_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallger_WADFILES_P): $(langallger_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallita_DATFILES_C = \
    ita/guitext.dat

langallita_WADFILES_S = \
    ita/alltext-s.wad \
    ita/alltext-s.idx

langallita_WADFILES_Z = \
    ita/alltext-z.wad \
    ita/alltext-z.idx

langallita_WADFILES_P = \
    ita/alltext-p.wad \
    ita/alltext-p.idx

langallita_POFILES_C = \
    guitext_comm_ita.po

langallita_POFILES_S = \
	alltext_syndct_ita.po \
	alltext_comm_ita.po

langallita_POFILES_Z = \
	alltext_church_ita.po \
	alltext_comm_ita.po

langallita_POFILES_P = \
	alltext_punks_ita.po \
	alltext_comm_ita.po

langallitadir = $(prefix)/$(PACKAGE)/language/ita
langallita_DATA = $(langallita_DATFILES_C) $(langallita_WADFILES_S) $(langallita_WADFILES_Z) $(langallita_WADFILES_P)

$(langallita_DATFILES_C): $(langallita_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 2,$(langall_eu_ETBLS))" -d $@ -c $+

$(langallita_WADFILES_S): $(langallita_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallita_WADFILES_Z): $(langallita_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallita_WADFILES_P): $(langallita_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallspa_DATFILES_C = \
    spa/guitext.dat

langallspa_WADFILES_S = \
    spa/alltext-s.wad \
    spa/alltext-s.idx

langallspa_WADFILES_Z = \
    spa/alltext-z.wad \
    spa/alltext-z.idx

langallspa_WADFILES_P = \
    spa/alltext-p.wad \
    spa/alltext-p.idx

langallspa_POFILES_C = \
    guitext_comm_spa.po

langallspa_POFILES_S = \
	alltext_syndct_spa.po \
	alltext_comm_spa.po

langallspa_POFILES_Z = \
	alltext_church_spa.po \
	alltext_comm_spa.po

langallspa_POFILES_P = \
	alltext_punks_spa.po \
	alltext_comm_spa.po

langallspadir = $(prefix)/$(PACKAGE)/language/spa
langallspa_DATA = $(langallspa_DATFILES_C) $(langallspa_WADFILES_S) $(langallspa_WADFILES_Z) $(langallspa_WADFILES_P)

$(langallspa_DATFILES_C): $(langallspa_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

$(langallspa_WADFILES_S): $(langallspa_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallspa_WADFILES_Z): $(langallspa_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallspa_WADFILES_P): $(langallspa_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

langallswe_DATFILES_C = \
    swe/guitext.dat

langallswe_WADFILES_S = \
    swe/alltext-s.wad \
    swe/alltext-s.idx

langallswe_WADFILES_Z = \
    swe/alltext-z.wad \
    swe/alltext-z.idx

langallswe_WADFILES_P = \
    swe/alltext-p.wad \
    swe/alltext-p.idx

langallswe_POFILES_C = \
    guitext_comm_swe.po

langallswe_POFILES_S = \
	alltext_syndct_swe.po \
	alltext_comm_swe.po

langallswe_POFILES_Z = \
	alltext_church_swe.po \
	alltext_comm_swe.po

langallswe_POFILES_P = \
	alltext_punks_swe.po \
	alltext_comm_swe.po

langallswedir = $(prefix)/$(PACKAGE)/language/swe
langallswe_DATA = $(langallswe_DATFILES_C) $(langallswe_WADFILES_S) $(langallswe_WADFILES_Z) $(langallswe_WADFILES_P)

$(langallswe_DATFILES_C): $(langallswe_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

$(langallswe_WADFILES_S): $(langallswe_POFILES_S)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallswe_WADFILES_Z): $(langallswe_POFILES_Z)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

$(langallswe_WADFILES_P): $(langallswe_POFILES_P)
	$(MKDIR_P) "$(@D)"
	$(TEXTWAD) -t "$(word 1,$(langall_eu_ETBLS))" -m "$(word 2,$(langall_eu_ETBLS))" -w $@ -c $+

EXTRA_DIST = \
	langallfre \
	langallger \
	langallita \
	langallspa \
	langallswe \
	langalleng
