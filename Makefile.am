ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = bflibrary bfsoundlib conf lang src util

# Do not install static libraries.
install installdirs: SUBDIRS = conf lang src util

doc_DATA = AUTHORS COPYING GPL3
docdir = $(prefix)/$(PACKAGE)

res_DATA =
resdir = $(prefix)/$(PACKAGE)

if TARGET_WINDOWS
res_DATA += res/swars_icon.ico
else
res_DATA += res/swars.icns res/swars_icon.png
endif

instscript_DATA = util/install
instscriptdir = $(prefix)/$(PACKAGE)

EXTRA_DIST = doc res instscript
