#include "core_check.h"

int main(void) {
	SRunner *sr = srunner_create(suite_check_core());
	srunner_run_all(sr, CK_SILENT);
	srunner_free(sr);
}
