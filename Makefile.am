# Use m4 macros from m4
ACLOCAL_AMFLAGS = -I m4 --install
EXTRA_DIST = m4/NOTES

# Note that we specify the include directories for the bundled dependencies
# here.
AM_CPPFLAGS = \
		-I$(top_srcdir)/src \
		-I$(top_srcdir)/build_lib/sources/spf2/src/include \
		-I$(top_srcdir)/build_lib/sources/clamav/libclamav \
		-I$(top_srcdir)/build_lib/sources/mysql/include \
		-I$(top_srcdir)/build_lib/sources/openssl/include \
		-I$(top_srcdir)/build_lib/sources/lzo/include \
		-I$(top_srcdir)/build_lib/sources/xml2/include \
		-I$(top_srcdir)/build_lib/sources/zlib \
		-I$(top_srcdir)/build_lib/sources/bzip2 \
		-I$(top_srcdir)/build_lib/sources/tokyocabinet \
		-I$(top_srcdir)/build_lib/sources/memcached \
		-I$(top_srcdir)/build_lib/sources/dkim/libopendkim \
		-I$(top_srcdir)/build_lib/sources/dspam/src \
		-I$(top_srcdir)/build_lib/sources/jansson/src \
		-I$(top_srcdir)/build_lib/sources/gd \
		-I$(top_srcdir)/build_lib/sources/png \
		-I$(top_srcdir)/build_lib/sources/jpeg \
		-I$(top_srcdir)/build_lib/sources/freetype/include

noinst_LIBRARIES = \
		libnetwork.a
#		libmagmacore.a \
#		libmagmacore_check.a \
#		libproviders.a

#libmagmacore_a_CPPFLAGS = -I$(top_srcdir)/src/core
#libmagmacore_a_SOURCES = \
#		src/core/indexes/cursors.c \
#		src/core/indexes/hashed.c \
#		src/core/indexes/inx.c \
#		src/core/indexes/linked.c \
#		src/core/memory/align.c \
#		src/core/memory/bits.c \
#		src/core/memory/memory.c \
#		src/core/memory/secure.c \
#		src/core/log/log.c \
#		src/core/thread/keys.c \
#		src/core/thread/mutex.c \
#		src/core/thread/rwlock.c \
#		src/core/thread/thread.c \
#		src/core/hash/adler.c \
#		src/core/hash/crc.c \
#		src/core/hash/fletcher.c \
#		src/core/hash/murmur.c \
#		src/core/classify/ascii.c \
#		src/core/host/files.c \
#		src/core/host/folder.c \
#		src/core/host/host.c \
#		src/core/host/mappings.c \
#		src/core/host/process.c \
#		src/core/host/spool.c \
#		src/core/buckets/arrays.c \
#		src/core/buckets/pool.c \
#		src/core/buckets/stacked.c \
#		src/core/strings/allocation.c \
#		src/core/strings/data.c \
#		src/core/strings/info.c \
#		src/core/strings/length.c \
#		src/core/strings/multi.c \
#		src/core/strings/nuller.c \
#		src/core/strings/opts.c \
#		src/core/strings/print.c \
#		src/core/strings/replace.c \
#		src/core/strings/shortcuts.c \
#		src/core/strings/validate.c \
#		src/core/compare/ends.c \
#		src/core/compare/equal.c \
#		src/core/compare/search.c \
#		src/core/compare/starts.c \
#		src/core/encodings/base64.c \
#		src/core/encodings/hex.c \
#		src/core/encodings/mappings.c \
#		src/core/encodings/qp.c \
#		src/core/encodings/url.c \
#		src/core/encodings/zbase32.c \
#		src/core/parsers/formats/nvp.c \
#		src/core/parsers/numbers/digits.c \
#		src/core/parsers/numbers/numbers.c \
#		src/core/parsers/special/bracket.c \
#		src/core/parsers/case.c \
#		src/core/parsers/line.c \
#		src/core/parsers/time.c \
#		src/core/parsers/token.c \
#		src/core/parsers/trim.c \
#		src/core/limits/system_limits.c
#
#libmagmacore_check_a_CPPFLAGS = -I$(top_srcdir)/src/core -I$(top_srcdir)/check/core
#libmagmacore_check_a_SOURCES = \
#		check/core/core_check.c
#
#check_PROGRAMS = core_tests
#
#core_tests_CPPFLAGS = $(CHECK_CFLAGS)
#core_tests_LDFLAGS = $(CHECK_LIBS)
#core_tests_SOURCES = \
#		check/core/runner.c
#core_tests_LDADD = libmagmacore_check.a libmagmacore.a

libnetwork_a_SOURCES = \
		src/network/addresses.c \
		src/network/clients.c \
		src/network/connections.c \
		src/network/listeners.c \
		src/network/options.c \
		src/network/read.c \
		src/network/reverse.c \
		src/network/write.c
#libproviders_a_SOURCES = \
#		src/providers/checkers/clamav.c
