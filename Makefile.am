ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = bflibrary bfsoundlib conf lang src util

# Do not install static libraries.
install installdirs: SUBDIRS = conf lang src util

doc_DATA = AUTHORS COPYING
docdir = $(prefix)/$(PACKAGE)

EXTRA_DIST = GPL3 doc res
