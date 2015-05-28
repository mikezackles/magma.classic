/**
 * @file /magma/core/limits/system_limits.h
 *
 * @brief Functions for querying system limits.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_CORE_SYSTEM_LIMITS_H
#define MAGMA_CORE_SYSTEM_LIMITS_H

uint64_t system_limit_cur(int_t resource);
uint64_t system_limit_max(int_t resource);

#endif
