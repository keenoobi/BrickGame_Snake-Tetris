#include "test.h"

START_TEST(record_save_test1) {
  int high_score = 1337;
  ck_assert_int_eq(saveHighestScore(&high_score, RECORDS_FILE), true);
}
END_TEST

START_TEST(record_save_test2) {
  ck_assert_int_eq(saveHighestScore(NULL, RECORDS_FILE), false);
}
END_TEST

START_TEST(record_load_test0) {
  int high_score = 9999;
  saveHighestScore(&high_score, RECORDS_FILE);
  ck_assert_int_eq(loadHighestScore(&high_score, RECORDS_FILE), true);
  ck_assert_int_eq(high_score, 9999);
}
END_TEST

START_TEST(record_load_test1) {
  ck_assert_int_eq(loadHighestScore(NULL, RECORDS_FILE), false);
}
END_TEST

Suite *record_suite(void) {
  Suite *s;
  TCase *tc_main;

  s = suite_create("record tests");
  tc_main = tcase_create("main");

  tcase_add_test(tc_main, record_save_test1);
  tcase_add_test(tc_main, record_save_test2);
  tcase_add_test(tc_main, record_load_test0);
  tcase_add_test(tc_main, record_load_test1);

  suite_add_tcase(s, tc_main);
  return s;
}