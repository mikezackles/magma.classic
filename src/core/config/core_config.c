
/**
 *
 * @file /magma/engine/config/global/global.c
 *
 * @brief	Functions for handling the global configuration.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */
#include "core.h"
#include "keys.h"

core_config_t core_config = {
	.config.secure.memory.enable = true
	// TODO!! - there are uninitialized values here right now!!
};
