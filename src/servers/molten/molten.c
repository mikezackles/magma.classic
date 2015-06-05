
/**
 * @file /magma/servers/molten/molten.c
 *
 * @brief	Functions used to handle Molten commands/actions.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "servers/servers.h"

#include "engine/status/status.h"
#include "engine/controller/controller.h"

void molten_stats(connection_t *con) {

	size_t length;

	length = stats_get_count();

	for(size_t i = 0; i < length; i++) {

		if (con_print(con, "STAT %s %lu\r\n", stats_get_name(i), stats_get_value_by_num(i)) < 0) {
			enqueue(&molten_quit, con);
			return;
		}

	}

	length = derived_count();

	for(size_t i = 0; i < length; i++) {

		if (con_print(con, "STAT %s %lu\r\n", derived_name(i), derived_value(i)) < 0) {
			enqueue(&molten_quit, con);
			return;
		}

	}

	con_write_bl(con, "END\r\n", 5) < 0 ? enqueue(&molten_quit, con) : enqueue(&molten_parse, con);

	return;
}

void molten_invalid(connection_t *con) {

	con_write_bl(con, "ERROR\r\n", 7) < 0 ? enqueue(&molten_quit, con) : enqueue(&molten_parse, con);
	return;
}

void molten_quit(connection_t *con) {

	con_destroy(con);
	return;
}

void molten_init(connection_t *con) {

	enqueue(&molten_parse, con);
	return;
}
