
/**
 * @file /magma/network/connections.c
 *
 * @brief	A collection of functions to allocate, configure and destroy connection structures.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "network/network.h"

#include "engine/status/status.h"
#include "servers/pop/pop.h"
#include "servers/imap/imap.h"
#include "servers/http/http.h"
#include "servers/smtp/smtp.h"
#include "servers/molten/molten.h"
#include "providers/cryptography/cryptography.h"

/**
 * @brief	Return the security level of a specified connection.
 * @param	con		the input client connection.
 * @return	1 for a secure connection, 0 for insecure, and -1 if the server does not support SSL/TLS.
 */
int_t con_secure(connection_t *con) {

	if (con && con->network.ssl) {
		return 1;
	}
	else if (con && con->server && con->server->ssl.context) {
		return 0;
	}

	return -1;
}

/**
 * @brief	Return the status of a specified connection.
 * @param	con		the input client connection.
 * @return	-1 on error, 0 for unknown status, 1 for connected, or 2 if the socket has been shutting down.
 */
int_t con_status(connection_t *con) {

	int_t result = -1;

	if (con && con->network.sockd != -1) {
		result = con->network.status;
	}

	return result;
}

/**
 * @brief	Destroy and free a generic connection object after executing its protocol-specific destructor; update any statistics accordingly.
 * @param	con		a pointer to the connection to be destroyed.
 * @return	This function returns no value.
 */
void con_destroy(connection_t *con) {

	if (con && !con_decrement_refs(con)) {

		switch (con->server->protocol) {
			case (POP):

				if (con->network.ssl) {
					stats_decrement_by_name("pop.connections.secure");
				}

				stats_decrement_by_name("pop.connections.total");
				pop_session_destroy(con);
				break;
			case (IMAP):
				if (con->network.ssl) {
					stats_decrement_by_name("imap.connections.secure");
				}

				stats_decrement_by_name("imap.connections.total");
				imap_session_destroy(con);
				break;
			case (HTTP):
				if (con->network.ssl) {
					stats_decrement_by_name("http.connections.secure");
				}

				stats_decrement_by_name("http.connections.total");
				http_session_destroy(con);
				break;
			case (SMTP):
				if (con->network.ssl) {
					stats_decrement_by_name("smtp.connections.secure");
				}

				stats_decrement_by_name("smtp.connections.total");
				smtp_session_destroy(con);
				break;
			case (SUBMISSION):
				if (con->network.ssl) {
					stats_decrement_by_name("smtp.connections.secure");
				}

				stats_decrement_by_name("smtp.connections.total");
				smtp_session_destroy(con);
				break;
			case (MOLTEN):
				if (con->network.ssl) {
					stats_decrement_by_name("molten.connections.secure");
				}

				stats_decrement_by_name("molten.connections.total");
				molten_session_destroy(con);
				break;
			default:
				break;
		}

		if (con->network.ssl) {
			ssl_free(con->network.ssl);
		}

		if (con->network.sockd != -1) {
			close(con->network.sockd);
		}

		st_cleanup(con->network.buffer);
		st_cleanup(con->network.reverse.domain);
		mutex_destroy(&(con->lock));
		mm_free(con);
	}

	return;
}

/**
 * @brief	Increment a connection object's reference counter.
 * @param	con		the connection object being referenced.
 * @return	an integer containing the new number of references to the specified connection.
 */
uint64_t con_increment_refs(connection_t *con) {

	uint64_t refs;

	mutex_lock(&(con->lock));
	refs = ++(con->refs);
	mutex_unlock(&(con->lock));

	return refs;
}

/**
 * @brief	Decrement a connection object's reference counter.
 * @param	con		the connection object being referenced.
 * @return	an integer containing the new number of references to the specified connection.
 */
uint64_t con_decrement_refs(connection_t *con) {

	uint64_t refs;

	mutex_lock(&(con->lock));
	refs = --(con->refs);
	mutex_unlock(&(con->lock));

	return refs;
}

/**
 * @brief	Allocate and initialize a new network buffer for a connection, if it is not already associated with one.
 * @note	A network buffer of size magma.system.network_buffer bytes will be allocated for the connection if one does not exist.
 * @return	true if the connection object has been associated with a network buffer or false on failure.
 */
bool_t con_init_network_buffer(connection_t *con) {

	if (!con) {
		return false;
	}
	else if (con->network.buffer) {
		return true;
	}

	if (!(con->network.buffer = st_alloc(magma.system.network_buffer))) {
		log_info("Unable to allocate a network buffer of %u bytes.", magma.system.network_buffer);
		return false;
	}

	con->network.line = pl_null();

	return true;
}

/**
 * @brief	Create a new connection object for a client connection.
 * @param	cond	the socket descriptor of the inbound client connection that was just accepted.
 * @param	server	the handle of the server that serviced the inbound request.
 */
connection_t * con_init(int cond, server_t *server) {

	connection_t *con;

	if (!(con = mm_alloc(sizeof(connection_t)))) {
		return NULL;
	}
	else if (mutex_init(&(con->lock), NULL)) {
		mm_free(con);
		return NULL;
	}

	con->network.sockd = cond;
	con->server = server;
	con_increment_refs(con);

	return con;
}
