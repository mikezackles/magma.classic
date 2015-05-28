# Use m4 macros from m4
ACLOCAL_AMFLAGS = -I m4 --install
EXTRA_DIST = m4/NOTES

AM_CPPFLAGS = -I$(top_srcdir)/src/core
noinst_LIBRARIES = libmagmacore.a
libmagmacore_a_SOURCES = \
		src/core/indexes/cursors.c \
		src/core/indexes/hashed.c \
		src/core/indexes/inx.c \
		src/core/indexes/linked.c \
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
		src/core/buckets/stacked.c \
		src/core/strings/allocation.c \
		src/core/strings/data.c \
		src/core/strings/info.c \
		src/core/strings/length.c \
		src/core/strings/multi.c \
		src/core/strings/nuller.c \
		src/core/strings/opts.c \
		src/core/strings/print.c \
		src/core/strings/replace.c \
		src/core/strings/shortcuts.c \
		src/core/strings/validate.c \
		src/core/compare/ends.c \
		src/core/compare/equal.c \
		src/core/compare/search.c \
		src/core/compare/starts.c \
		src/core/encodings/base64.c \
		src/core/encodings/hex.c \
		src/core/encodings/mappings.c \
		src/core/encodings/qp.c \
		src/core/encodings/url.c \
		src/core/encodings/zbase32.c \
		src/core/parsers/formats/nvp.c \
		src/core/parsers/numbers/digits.c \
		src/core/parsers/numbers/numbers.c \
		src/core/parsers/special/bracket.c \
		src/core/parsers/case.c \
		src/core/parsers/line.c \
		src/core/parsers/time.c \
		src/core/parsers/token.c \
		src/core/parsers/trim.c \
		src/core/limits/system_limits.c
