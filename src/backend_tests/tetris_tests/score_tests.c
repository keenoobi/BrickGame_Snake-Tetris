#include "test.h"

START_TEST(score_loop_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  params_t prms;
  prms.stats = tetris;
  prms.stats->score = 0;
  countPoints(&prms, _i);

  if (_i == 1) ck_assert_int_eq(prms.stats->score, 100);
  if (_i == 2) ck_assert_int_eq(prms.stats->score, 300);
  if (_i == 3) {
    ck_assert_int_eq(prms.stats->score, 700);
    ck_assert_int_eq(prms.stats->level, 1);
  }
  if (_i == 4) {
    ck_assert_int_eq(prms.stats->score, 1500);
    ck_assert_int_eq(prms.stats->level, 2);
  }
}
END_TEST

START_TEST(score_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  params_t prms;
  prms.stats = tetris;
  prms.stats->score = 0;
  int lines = -1;
  countPoints(&prms, lines);

  ck_assert_int_eq(prms.stats->score, 0);
  ck_assert_int_eq(prms.stats->level, 0);
}
END_TEST

START_TEST(score_test1) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  params_t prms;
  prms.stats = tetris;
  prms.stats->score = 0;
  int lines = 0;
  countPoints(&prms, lines);

  ck_assert_int_eq(prms.stats->score, 0);
  ck_assert_int_eq(prms.stats->level, 0);
}
END_TEST

START_TEST(score_test2) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  params_t prms;
  prms.stats = tetris;
  prms.stats->score = 0;
  prms.stats->record = 0;
  int lines = 4;
  countPoints(&prms, lines);

  ck_assert_int_eq(prms.stats->score, 1500);
  ck_assert_int_eq(prms.stats->level, 2);
}
END_TEST

Suite *score_suite(void) {
  Suite *s;
  TCase *tc_main;

  s = suite_create("score tests");
  tc_main = tcase_create("main");

  tcase_add_test(tc_main, score_test0);
  tcase_add_test(tc_main, score_test1);
  tcase_add_test(tc_main, score_test2);

  tcase_add_loop_test(tc_main, score_loop_test0, 1, 5);

  suite_add_tcase(s, tc_main);
  return s;
}