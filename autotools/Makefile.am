MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure COPYING \
					   INSTALL config.h.in config

ACLOCAL_AMFLAGS =-I m4 -static -lpthread


AC_CFLAGS=-$(AC_CFLAGS) -static -lpthread

maintainer-clean-local:
	-rm -rf config
	-rm -r m4

bin_PROGRAMS = src/cryptprog/cryptprog

cryptprog_SOURCES = src/cryptprog/cryptprog.c

update:
	cp src/cryptprog/cryptprog ../install_folder/
	make clean
