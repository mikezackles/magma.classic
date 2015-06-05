
/**
 * @file /magma/engine/config/config.h
 *
 * @brief	The entry point for all configuration modules used by magma.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_ENGINE_OPTIONS_H
#define MAGMA_ENGINE_OPTIONS_H

// Macros because we have a lot of these checks
#define CONFIG_CHECK_EXISTS(option,ptype)	if (option && !file_accessible(option)) { \
		log_critical(#ptype " specified in " #option " is not accessible: { path = %s, error = %s }", option, strerror_r(errno, bufptr, buflen)); \
		result = false; \
	}
#define CONFIG_CHECK_FILE_READABLE(x)	CONFIG_CHECK_EXISTS(x,"Filename")
#define CONFIG_CHECK_DIR_READABLE(x)	CONFIG_CHECK_EXISTS(x,"Directory")
#define CONFIG_CHECK_READWRITE(option,ptype)	if (option && !file_readwritable(option)) { \
		log_critical(#ptype " specified in " #option " is not accessible for reading and writing: { path = %s, error = %s }", option, strerror_r(errno, bufptr, buflen)); \
		result = false; \
	}
#define CONFIG_CHECK_FILE_READWRITE(x)	CONFIG_CHECK_READWRITE(x,"Filename")
#define CONFIG_CHECK_DIR_READWRITE(x)	CONFIG_CHECK_READWRITE(x,"Directory")



#include "cache/cache.h"
#include "relay/relay.h"
#include "servers/servers.h"
#include "global/global.h"

#endif
