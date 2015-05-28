# Use m4 macros from m4
ACLOCAL_AMFLAGS = -I m4 --install
EXTRA_DIST = m4/NOTES

AM_CPPFLAGS = -I$(top_srcdir)/src/core
noinst_LIBRARIES = libmagmacore.a
libmagmacore_a_SOURCES = \
		src/core/memory/align.c \
		src/core/memory/bits.c \
		src/core/memory/memory.c \
		src/core/memory/secure.c \
		src/core/log/log.c \
		src/core/thread/keys.c \
		src/core/thread/mutex.c \
		src/core/thread/rwlock.c \
		src/core/thread/thread.c \
		src/core/hash/adler.c \
		src/core/hash/crc.c \
		src/core/hash/fletcher.c \
		src/core/hash/murmur.c \
		src/core/classify/ascii.c \
        src/core/host/files.c \
        src/core/host/folder.c \
        src/core/host/host.c \
        src/core/host/mappings.c \
        src/core/host/process.c \
        src/core/host/spool.c \
        src/core/buckets/arrays.c \
        src/core/buckets/pool.c \
        src/core/buckets/stacked.c
