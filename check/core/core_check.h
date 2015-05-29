
/**
 * @file /magma.check/core/core_check.h
 *
 * @brief The entry point for the core module test suite.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef CORE_CHECK_H
#define CORE_CHECK_H

#include <check.h>

#define RUN_TEST_CASE_TIMEOUT 100
#define testcase(s, tc, name, func) tcase_add_test((tc = tcase_create(name)), func); tcase_set_timeout(tc, RUN_TEST_CASE_TIMEOUT); suite_add_tcase(s, tc)

//typedef struct {
//	inx_t *inx;
//	uint64_t type;
//} check_inx_opt_t;
//
//struct check_mi_t {
//	int nr;
//	char *name;
//};
//
//extern stringer_t *string_check_constant;
//
///// string_check.c
//bool_t   check_string_alloc(uint32_t check);
//bool_t   check_string_dupe(uint32_t check);
//bool_t   check_string_import(void);
//bool_t   check_string_merge(void);
//bool_t   check_string_print(void);
//bool_t   check_string_realloc(uint32_t check);
//
///// qp_check.c
//bool_t   check_encoding_qp(void);
//
///// inx_check.c
//bool_t    check_inx_cursor_mthread(check_inx_opt_t *opts);
//void		  check_inx_cursor_mthread_cnv(check_inx_opt_t *opts);
//bool_t    check_inx_cursor_sthread(check_inx_opt_t *opts);
//bool_t    check_inx_mthread(check_inx_opt_t *opts);
//void		  check_inx_mthread_cnv(check_inx_opt_t *opts);
//bool_t    check_inx_sthread(check_inx_opt_t *opts);
//
///// linked_check.c
//bool_t   check_indexes_linked_cursor(char **errmsg);
//bool_t   check_indexes_linked_cursor_compare(uint64_t values[], inx_cursor_t *cursor);
//bool_t   check_indexes_linked_simple(char **errmsg);
//
///// hex_check.c
//bool_t   check_encoding_hex(void);
//
///// url_check.c
//bool_t   check_encoding_url(void);

/// core_check.c
Suite *                    suite_check_core(void);

///// base64_check.c
//bool_t   check_encoding_base64(bool_t secure_on);
//bool_t   check_encoding_base64_mod(bool_t secure_on);
//
///// hashed_check.c
//bool_t   check_indexes_hashed_cursor(char **errmsg);
//bool_t   check_indexes_hashed_cursor_compare(uint64_t values[], inx_cursor_t *cursor);
//bool_t   check_indexes_hashed_simple(char **errmsg);
//
///// system_check.c
//bool_t   check_system_errnonames(void);
//bool_t   check_system_signames(void);
//
///// tree_check.c
//bool_t   check_indexes_tree_cursor(char **errmsg);
//bool_t   check_indexes_tree_cursor_compare(uint64_t values[], inx_cursor_t *cursor);
//bool_t   check_indexes_tree_simple(char **errmsg);
//
///// qsort_check.c
//int   check_bsearch_compare(const void *m1, const void *m2);
//int   check_bsearch_months(int num, char *name);
//
///// zbase32_check.c
//bool_t   check_encoding_zbase32(void);

#endif
