ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = bflibrary bfsoundlib conf lang src util

# Do not install static libraries.
install installdirs: SUBDIRS = conf lang src util

doc_DATA = AUTHORS COPYING GPL3
docdir = $(prefix)/$(PACKAGE)

res_DATA = res/swars.ico res/swars.icns
resdir = $(prefix)/$(PACKAGE)

instscript_DATA = util/install
instscriptdir = $(prefix)/$(PACKAGE)

EXTRA_DIST = doc res instscript
