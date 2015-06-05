
/**
 * @file /magma/engine/status/status.c
 *
 * @brief	Functions used to coordinate system state and worker thread operation and shutdown..
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "engine/engine.h"

struct {
	pid_t pid;
	uint64_t startup;
} process = {
	.pid = 0,
	.startup = 0
};

int status_level = 0;
pthread_mutex_t status_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief	Check to see if a worker thread should continue processing.
 * @note	This function should be called periodically by worker threads.
 * @return	true if the caller should continue processing (status level is positive) or false otherwise.
 */
bool_t status(void) {

	bool_t result = false;

	mutex_lock(&status_mutex);
	if (status_level >= 0) result = true;
	mutex_unlock(&status_mutex);

	return result;
}

/**
 * @brief	Set the status level to a specified value.
 * @see		status()
 * @param	value	the integer value of the new status level.
 * @return	This function returns no value.
 */
void status_set(int value) {
	mutex_lock(&status_mutex);
	status_level = value;
	mutex_unlock(&status_mutex);
	return;
}

/**
 * @brief	Get the current status level.
 * @see		status()
 * @return	the current value of the status level.
 */
int status_get(void) {
	int value;
	mutex_lock(&status_mutex);
	value = status_level;
	mutex_unlock(&status_mutex);
	return value;
}

/**
 * @brief	Update magma's basic process information (pid and start time).
 * @return	This function returns no value.
 */
void status_process(void) {
	process.pid = getpid();
	if (!process.startup) process.startup = time(NULL);
	return;
}

/**
 * @brief	Get the magma instance's pid.
 * @return	the value of magma's pid.
 */
uint64_t status_pid(void) {
	return process.pid;
}

/**
 * @brief	Get the timestamp corresponding to when magma was started.
 * @return	the UNIX-style date-time value when magma was started.
 */
uint64_t status_startup(void) {
	return process.startup;
}
