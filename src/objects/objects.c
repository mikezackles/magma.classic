
/**
 * @file /magma/objects/objects.c
 *
 * @brief	Functions used for managing objects.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "objects/objects.h"

#include "engine/status/status.h"

object_cache_t objects = {
	.users = NULL,
	.sessions = NULL
};

/**
 * @brief	Initialize the object cache for all active user objects and web sessions.
 * @return	true on success or false on failure.
 */
bool_t obj_cache_start(void) {

	if (!(objects.users = inx_alloc(M_INX_HASHED | M_INX_LOCK_MANUAL, &meta_user_destroy))) {
		log_critical("Unable to initialize the user information cache.");
		return false;
	}

	if (!(objects.sessions = inx_alloc(M_INX_HASHED | M_INX_LOCK_MANUAL, &sess_destroy))) {
		log_critical("Unable to initialize the session cache.");
		return false;
	}

	return true;
}

/**
 * @brief	Stop the object cache and free all active user and web session objects.
 * @return	This function returns no value.
 */
void obj_cache_stop(void) {

	// Since web sessions can contain user objects; we need to free the sessions first, otherwise we'll have memory access errors.
	if (objects.sessions) {
		inx_free(objects.sessions);
		objects.sessions = NULL;
	}

	if (objects.users) {
		inx_free(objects.users);
		objects.users = NULL;
	}

	return;
}

void obj_cache_prune(void) {

	time_t now;
	double_t gap;
	session_t *sess;
	meta_user_t *user;
	inx_cursor_t *cursor;
	uint64_t count, expired;

	if ((now = time(NULL)) == (time_t)(-1)) {
		return;
	}

	if (objects.users && (cursor = inx_cursor_alloc(objects.users))) {

		count = expired = 0;

		inx_lock_read(objects.users);

		// If were currently holding more than 4,096 users, prune those older than 5 minutes.
		if (inx_count(objects.users) > 4096) {
			gap = 300;
		}
		// Otherwise only prune those older than 1 hour.
		else  {
			gap = 3600;
		}

		inx_unlock(objects.users);
		inx_lock_write(objects.users);

		user = inx_cursor_value_next(cursor);

		while (user) {
			if (difftime(now, meta_user_ref_stamp(user)) > gap && !meta_user_ref_total(user)) {
				inx_delete(objects.users, inx_cursor_key_active(cursor));
				inx_cursor_reset(cursor);
				expired++;
			}
			user = inx_cursor_value_next(cursor);
		}

		// Record the total so we can update the statistics variable.
		count = inx_count(objects.users);
		inx_unlock(objects.users);
		inx_cursor_free(cursor);

		stats_set_by_name("objects.users.total", count);
		stats_adjust_by_name("objects.users.expired", expired);
	}

	if (objects.sessions && (cursor = inx_cursor_alloc(objects.sessions))) {

		count = expired = 0;

		inx_lock_read(objects.sessions);

		// If were currently holding more than 4,096 sessions, prune those older than 30 minutes.
		if (inx_count(objects.sessions) > 4096) {
			gap = 1800;
		}
		// Otherwise only prune those older than 1 hour.
		else  {
			gap = 3600;
		}

		inx_unlock(objects.sessions);
		inx_lock_write(objects.sessions);

		sess = inx_cursor_value_next(cursor);

		while (sess) {
			if (difftime(now, sess_ref_stamp(sess)) > gap && !sess_ref_total(sess)) {
				inx_delete(objects.sessions, inx_cursor_key_active(cursor));
				inx_cursor_reset(cursor);
				expired++;
			}
			sess = inx_cursor_value_next(cursor);
		}

		// Record the total so we can update the statistics variable.
		count = inx_count(objects.sessions);
		inx_unlock(objects.sessions);
		inx_cursor_free(cursor);

		stats_set_by_name("objects.sessions.total", count);
		stats_adjust_by_name("objects.sessions.expired", expired);
	}


	return;
}

