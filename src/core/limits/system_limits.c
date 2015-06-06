/**
 * @file /magma/core/limits/system_limits.c
 *
 * @brief Functions for querying system limits.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "magma_common.h"

/**
 * @brief	Get the hard system limit for a specified resource.
 * @note	This function will never return a value greater than UINT64_MAX.
 * @see		getrlimit64()
 * @param	resource	the system rlimit resource identifier to be queried.
 * @return	-1 on failure, or the system hard limit of the specified resource identifier on success.
 */
uint64_t system_limit_max(int_t resource) {

	int_t ret;
	struct rlimit64 limits = { 0, 0 };

	if ((ret = getrlimit64(resource, &limits))) {
		log_info("Unable to retrieve the resource limit. {resource = %i / return = %i / error = %s}", resource, ret, strerror_r(errno, bufptr, buflen));
		return -1;
	}

	if (limits.rlim_max > UINT64_MAX) {
		log_pedantic("The requested resource has a maximum value that exceeds the range of possible of return values. Returning the maximum possible value instead. "
			"{resource = %i / actual = %lu / returning = %lu}", resource, limits.rlim_max, UINT64_MAX);
		return UINT64_MAX;
	}

	return (uint64_t)limits.rlim_max;
}

/**
 * @brief	Get the soft system limit for a specified resource.
 * @note	This function will never return a value greater than UINT64_MAX.
 * @see		getrlimit64()
 * @param	resource	the system rlimit resource identifier to be queried.
 * @return	-1 on failure, or the system soft limit of the specified resource identifier on success.
 */
uint64_t system_limit_cur(int_t resource) {

	int_t ret;
	struct rlimit64 limits = { 0, 0 };

	if ((ret = getrlimit64(resource, &limits))) {

		log_info("Unable to retrieve the resource limit. {resource = %i / return = %i / error = %s}", resource, ret, strerror_r(errno, bufptr, buflen));
		return -1;
	}

	if (limits.rlim_cur > UINT64_MAX) {
		log_pedantic("The requested resource is currently set to a value that exceeds the range of possible of return values. Returning the maximum possible value instead. "
			"{resource = %i / actual = %lu / returning = %lu}", resource, limits.rlim_cur, UINT64_MAX);
		return UINT64_MAX;
	}

	return (uint64_t)limits.rlim_cur;
}
