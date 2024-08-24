#include "test.h"

int main(void) {
  int failed_count;
  SRunner *sr;

  sr = srunner_create(NULL);

  srunner_add_suite(sr, tetromino_suite());
  srunner_add_suite(sr, score_suite());
  srunner_add_suite(sr, record_suite());
  srunner_add_suite(sr, helpers_suite());

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  failed_count = srunner_ntests_failed(sr);

  srunner_free(sr);

  return !failed_count ? EXIT_SUCCESS : EXIT_FAILURE;
}