# Use m4 macros from m4
ACLOCAL_AMFLAGS = -I m4 --install
EXTRA_DIST = m4/NOTES

AM_CPPFLAGS = -I$(top_srcdir)/src/core
noinst_LIBRARIES = libmagmacore.a
libmagmacore_a_SOURCES = src/core/memory/align.c
