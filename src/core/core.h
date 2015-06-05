
/**
 * @file /magma/core/core.h
 *
 * @brief	A collection of types, declarations and includes needed when accessing the core module and the type definitions needed to parse the header files that follow.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_CORE_H
#define MAGMA_CORE_H

#define MAGMA_CORE_FILEPATH_MAX PATH_MAX

/**
 * Different types used throughout.
 */
typedef enum {
	M_TYPE_MULTI = 1,   //!< M_TYPE_MULTI is multi_t
	M_TYPE_ENUM,		//!< M_TYPE_ENUM is enum
	M_TYPE_BOOLEAN, //!< M_TYPE_BOOLEAN is bool_t
	M_TYPE_BLOCK,   //!< M_TYPE_BLOCK is void pointer
	M_TYPE_NULLER,  //!< M_TYPE_NULLER is char pointer
	M_TYPE_PLACER,  //!< M_TYPE_PLACER is placer_t struct
	M_TYPE_STRINGER,//!< M_TYPE_STRINGER is stringer_t pointer
	M_TYPE_INT8,    //!< M_TYPE_INT8 is int8_t
	M_TYPE_INT16,   //!< M_TYPE_INT16 is int16_t
	M_TYPE_INT32,   //!< M_TYPE_INT32 is int32_t
	M_TYPE_INT64,   //!< M_TYPE_INT64 is int64_t
	M_TYPE_UINT8,   //!< M_TYPE_UINT8 is uint8_t
	M_TYPE_UINT16,  //!< M_TYPE_UINT16 is uint16_t
	M_TYPE_UINT32,  //!< M_TYPE_UINT32 is uint32_t
	M_TYPE_UINT64,  //!< M_TYPE_UINT64 is uint64_t
	M_TYPE_FLOAT,   //!< M_TYPE_FLOAT is float
	M_TYPE_DOUBLE   //!< M_TYPE_DOUBLE is double
} M_TYPE;

enum {
	EMPTY = 0
};

/************ TYPE ************/
char * type(M_TYPE type);
/************ TYPE ************/

#include "memory/memory.h"
#include "strings/strings.h"
#include "classify/classify.h"
#include "encodings/encodings.h"
#include "log/log.h"
#include "indexes/indexes.h"
#include "compare/compare.h"
#include "thread/thread.h"
#include "buckets/buckets.h"
#include "parsers/parsers.h"
#include "hash/hash.h"
#include "host/host.h"
#include "limits/system_limits.h"

extern __thread char coreThreadBuffer[1024];
#define corebufptr (char *)&(coreThreadBuffer)
#define corebuflen sizeof(coreThreadBuffer)

#endif
