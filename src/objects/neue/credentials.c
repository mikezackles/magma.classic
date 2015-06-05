
/**
 * @file /magma/objects/users/credentials.c
 *
 * @brief Functions used for track and update the object checkpoints.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#include "objects/objects.h"

#include "providers/cryptography/cryptography.h"

/// LOW: Add a function for detecting potentially illegal username/address sequences. Valid usernames must start with an alpha character,
/// end with an alphanumeric character and not user consecutive underscores. If present, the domain portion of the username must follow the
/// applicable standard for the TLD being used.

/// LOW: Figure out how to result a credential_address into a credential_username using the Mailboxes table.


/**
 * @brief	Process a user supplied credential address to ensures it only contains valid characters.
 * @param	s	a managed string containing the user's credential address to be processed, with an optional domain suffix.
 * @note	This function duplicates the input address string, with all characters converted to lowercase, and whitespace removed.
 * 			'.' and '-' are also converted to '_' in the username, and if there is a '+' in the username portion of the
 * 			credential address, all subsequent characters in that username will be ignored.
 * @return	NULL on failure, or a managed string containing the validated credential address on success.
 */
stringer_t * credential_address(stringer_t *s) {

	size_t len;
	chr_t *p, *w, *tip = NULL;
	stringer_t *output, *handle, *domain = NULL;

	if (st_empty_out(s, (uchr_t **)&p, &len) || !(output = st_alloc_opts(MANAGED_T | CONTIGUOUS | SECURE, len + 1))) {
		return NULL;
	}

	w = st_char_get(output);

	for (size_t i = 0; i < len; i++) {

		// If an @ symbol is encountered, record its position
		if (!tip && *p == '@') {
			tip = w;
		}

		// Non white space characters are copied in lower case form
		if (!chr_whitespace(*p)) {
			*w++ = lower_chr(*p++);
		}

		// Just advance past white space
		else {
			p++;
		}

	}

	st_length_set(output, w - st_char_get(output));

	// If an @ symbol was encountered, save the handle and domain portions separately since they use different rules.
	if (tip) {
		handle = PLACER(st_char_get(output), tip - st_char_get(output));
		domain = PLACER(tip, st_length_get(output) - st_length_get(handle));
	}
	else {
		handle = PLACER(st_char_get(output), st_length_get(output));
	}

	p = st_char_get(handle);
	len = st_length_get(handle);
	tip = NULL;

	for (size_t i = 0; !tip && i < len; i++) {

		// Save the location of a plus sign (+) so it can be be stripped from the handle section.
		if (*p == '+') {
			tip = p;
		}
		// Translate periods and dashes to underscores allowing them to be used interchanged if the user desires.
		else if (*p == '.' || *p == '-') {
			*p++ = '_';
		}
		else {
			p++;
		}

	}

	// If a plus sign was found in the handle, trim the handle and if necessary shift the domain portion to accommodate.
	if (tip && domain) {
		w = st_char_get(domain);
		len = st_length_get(domain);

		for (size_t i = 0; i < len; i++) {
			*p++ = *w++;
		}

		st_length_set(output, (size_t)(p - st_char_get(output)));
	}
	else if (tip) {
		st_length_set(output, (size_t)(p - st_char_get(output)));
	}

	return output;
}

/**
 * @brief	Get the valid credential username portion of a fully qualified user address.
 * @param	s	a managed string containing the user's credential address to be processed, with an optional domain suffix.
 * @return	NULL on failure, or a managed string containing the credential username portion of the supplied address.
 */
stringer_t * credential_username(stringer_t *s) {

	size_t at;
	stringer_t *output, *domain = NULL;

	if (!(output = credential_address(s))) {
		return NULL;
	}

	// If an @ symbol was encountered, check the domain portion to see if it matches magma.system.domain and if so strip it off.
	if (st_search_cs(output, PLACER("@", 1), &at) && (st_length_get(output) - at) > 1) {
		domain = PLACER(st_char_get(output) + at + 1, st_length_get(output) - at - 1);
		if (!st_cmp_cs_eq(domain, magma.system.domain)) {
			st_length_set(output, at);
		}
	}

	return output;
}

/**
 * @brief	Free a user credential.
 * @param	cred	a pointer to the user credential object to be freed.
 * @return	This function returns no value.
 */
void credential_free(credential_t *cred) {

	if (cred) {

		if (cred->type == CREDENTIAL_AUTH) {
			st_cleanup(cred->auth.username);
			st_cleanup(cred->auth.domain);
			st_cleanup(cred->auth.password);
			st_cleanup(cred->auth.key);
		}

		else if (cred->type == CREDENTIAL_MAIL) {
			st_cleanup(cred->mail.address);
			st_cleanup(cred->mail.domain);
		}

		mm_free(cred);
	}

	return;
}

/**
 * @brief	Get a user mail credential for anonymous use within the mail subsystem.
 * @note	This function is used by the smtp service to fetch the smtp inbound preferences of a recipient user.
 * @param	address		a managed string containing the email address of the user account.
 * @return	NULL on failure, or a pointer to the requested user's mail credentials on success.
 */
credential_t * credential_alloc_mail(stringer_t *address) {

	size_t at;
	credential_t *result = NULL;

	if (!(result = mm_alloc(sizeof(credential_t)))) {
		return NULL;
	}

	// Boil the address
	if ((result->type = CREDENTIAL_MAIL) != CREDENTIAL_MAIL || !(result->mail.address = credential_address(address)) ||
		// QUESTION: What is mail.domain being used for?
		!(result->mail.domain = st_alloc_opts(PLACER_T | JOINTED | SECURE | FOREIGNDATA, 0))) {
		credential_free(result);
		return NULL;
	}

	// If applicable, track the domain portion of the username
	else if (st_search_cs(result->mail.address, PLACER("@", 1), &at) && at != 0 && (st_length_get(result->mail.address) - at) > 2) {
		st_data_set(result->auth.domain, st_data_get(result->auth.username) + at + 1);
		st_length_set(result->auth.domain, st_length_get(result->auth.username) - at - 1);
	}

	// Reject addresses without both a mailbox and domain by returning NULL.
	else {
		credential_free(result);
		return NULL;
	}

	return result;
}

/**
 * @brief	Construct a user credential object from supplied username and password.
 * @note	The credential's auth.key field becomes a single pass hash of the password, while auth.password is created from a three-time hash.
 * @param	username	the input username.
 * @param	password	the plaintext password of the user.
 * @return	NULL on failure, or a pointer to the requested user's auth credentials on success.
 */
credential_t * credential_alloc_auth(stringer_t *username, stringer_t *password) {

	size_t at;
	credential_t *result = NULL;
	stringer_t *binary, *sanitized, *combo[3] = { NULL, NULL, NULL };

	if (st_empty(username) || st_empty(password) || !(sanitized = credential_address(username))) {
		return NULL;
	}

	if (!(result = mm_alloc(sizeof(credential_t)))) {
		st_free(sanitized);
		return NULL;
	}

	// Boil the username
	if (!(result->type = CREDENTIAL_AUTH) || !(result->auth.password = st_alloc_opts(MANAGED_T | CONTIGUOUS | SECURE, 129)) ||
		!(result->auth.key = st_alloc_opts(MANAGED_T | CONTIGUOUS | SECURE, 129)) || !(result->auth.username = credential_username(sanitized))) {
		credential_free(result);
		st_free(sanitized);
		return NULL;
	}

	// Were done with the sanitized address.
	st_free(sanitized);

	// If applicable, look for the domain portion of the username.
	if (st_search_cs(result->auth.username, PLACER("@", 1), &at) && (st_length_get(result->auth.username) - at) > 1) {

		if (!(result->auth.domain = st_alloc_opts(MANAGED_T | JOINTED | SECURE, st_length_get(result->auth.username)))) {
			credential_free(result);
			return NULL;
		}

		st_copy_in(result->auth.domain, st_data_get(result->auth.username)+at+1, st_length_get(result->auth.username)-at-1);
	}
	// Otherwise set to the default domain.
	else if (!(result->auth.domain = st_merge_opts(MANAGED_T | JOINTED | SECURE, "s", magma.system.domain))) {
			credential_free(result);
			return NULL;
	}

	if (!(binary = st_alloc_opts(BLOCK_T | CONTIGUOUS | SECURE, 64))) {
		credential_free(result);
		return NULL;
	}
	// Build the key and then iterate two more times to build the password hash. If an error free the result, but continue so the
	// standard cleanup code can free any of the buffers that were used.
	else if (!(combo[0] = st_merge_opts(MANAGED_T | CONTIGUOUS | SECURE, "sss", result->auth.username, magma.secure.salt, password)) ||
		digest_sha512(combo[0], binary) != binary || hex_encode_st(binary, result->auth.key) != result->auth.key ||
		!(combo[1] = st_merge_opts(MANAGED_T | CONTIGUOUS | SECURE, "ss", password, binary)) || digest_sha512(combo[1], binary) != binary ||
		!(combo[2] = st_merge_opts(MANAGED_T | CONTIGUOUS | SECURE, "ss", password, binary)) || digest_sha512(combo[2], binary) != binary ||
		hex_encode_st(binary, result->auth.password) != result->auth.password) {
		credential_free(result);
		result = NULL;
	}

	st_cleanup(combo[2]);
	st_cleanup(combo[1]);
	st_cleanup(combo[0]);
	st_free(binary);

	return result;

}

