/**
 * @file /magma/magma_external.h
 *
 * @brief Include headers for bundled external deps.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_EXTERNAL_H
#define MAGMA_EXTERNAL_H

// SPF
#include "spf.h"
#include "spf_dns_zone.h"

// ClamAV
#include "clamav.h"

// MySQL
#include "mysql.h"

// OpenSSL
#include "openssl/conf.h"
#include "openssl/ssl.h"
#include "openssl/crypto.h"
#include "openssl/rand.h"
#include "openssl/engine.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"
#include "openssl/ec.h"
#include "openssl/dh.h"
#include "openssl/err.h"

// LZO
#include "lzo/lzoconf.h"
#include "lzo/lzodefs.h"
#include "lzo/lzoutil.h"
#include "lzo/lzo1x.h"

// XML2
#include "libxml/xmlmemory.h"
#include "libxml/tree.h"
#include "libxml/valid.h"
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"
#include "libxml/parserInternals.h"
#include "libxml/xmlerror.h"

// ZLIB
#include "zlib.h"

// BZIP
#include "bzlib.h"

// TOKYO
#include "tcutil.h"
#include "tcadb.h"
#include "tchdb.h"
#include "tcbdb.h"

// Memcached
#include "libmemcached/memcached.h"

// DKIM
#define lint
#include "dkim.h"
#undef lint

// DSPAM
#define CONFIG_DEFAULT ""
#define LOGDIR "~/"
#include "libdspam.h"
#include "mysql_drv.h"

// Jansson
#include "jansson.h"

// GD
#include "gd.h"

// PNG
#include "png.h"

// JPEG
#include "jpeglib.h"

// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

#endif
