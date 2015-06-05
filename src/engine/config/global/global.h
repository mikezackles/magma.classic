
/**
 * @file /magma/engine/config/global/global.h
 *
 * @brief	The global configuration structure used for overall system settings, and functions to initialize it at startup and free it at shutdown.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_ENGINE_CONFIG_GLOBAL_H
#define MAGMA_ENGINE_CONFIG_GLOBAL_H

#include "providers/database/database.h"

typedef struct {
	void *store; /* The location in memory to store the setting value. */
	multi_t norm; /* The default value. */
	chr_t *name; /* The search key/name of the setting. */
	chr_t *description; /* Description of the setting and its default value. */
	bool_t file; /* Can this value be set using the config file? */
	bool_t database; /* Can this value be set using the config file? */
	bool_t overwrite; /* Can this value be overwritten? */
	bool_t set; /* Has this setting already been provided? */
	bool_t required; /* Is this setting required? */
} magma_keys_t;

/// datatier.c
uint64_t   config_fetch_host_number(void);
table_t *  config_fetch_settings(void);

/// global.c
void            config_free(void);
magma_keys_t *  config_key_lookup(stringer_t *name);
bool_t          config_load_database_settings(void);
bool_t			config_load_cmdline_settings(void);
bool_t          config_load_defaults(void);
bool_t          config_load_file_settings(void);
void            config_output_help(void);
void            config_output_settings(void);
void			config_output_value_generic(chr_t *prefix, chr_t *name, M_TYPE type, void *val, bool_t required);
void            config_output_value(magma_keys_t *key);
bool_t          config_validate_settings(void);
bool_t          config_value_set(magma_keys_t *setting, stringer_t *value);

#endif
