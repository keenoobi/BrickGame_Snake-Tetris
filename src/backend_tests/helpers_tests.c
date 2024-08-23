#include "test.h"

START_TEST(helpers_rotation_loop_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  // newFallingFigure(tetris);

  prms.stats->falling_tetromino.type = _i;

  prms.stats->falling_tetromino.coordinates.col = 3 + _i;
  for (int i = 0; i < NUM_ORIENTATIONS; i++) {
    ck_assert_int_eq(checkRotationPossibility(
                         &prms, prms.stats->falling_tetromino.coordinates.col,
                         prms.stats->falling_tetromino.orient),
                     true);
    prms.stats->falling_tetromino.orient++;
  }
}
END_TEST

START_TEST(helpers_rotation_loop_test1) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  // newFallingFigure(tetris);

  prms.stats->falling_tetromino.type = _i;

  prms.stats->falling_tetromino.coordinates.col = 4 - _i;
  for (int i = 0; i < NUM_ORIENTATIONS; i++) {
    ck_assert_int_eq(checkRotationPossibility(
                         &prms, prms.stats->falling_tetromino.coordinates.col,
                         prms.stats->falling_tetromino.orient),
                     true);
    prms.stats->falling_tetromino.orient++;
  }
}
END_TEST

START_TEST(helpers_rotation_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 1;
  prms.stats->falling_tetromino.coordinates.row = 5;
  prms.stats->falling_tetromino.coordinates.col = 2;
  placeTetromino(tetris, tetris->falling_tetromino);
  newFallingFigure(tetris);
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 1;
  prms.stats->falling_tetromino.coordinates.row = 5;
  prms.stats->falling_tetromino.coordinates.col = 0;
  placeTetromino(tetris, tetris->falling_tetromino);
  prms.stats->falling_tetromino.orient = 2;

  ck_assert_int_eq(checkRotationPossibility(
                       &prms, prms.stats->falling_tetromino.coordinates.col,
                       prms.stats->falling_tetromino.orient),
                   false);
}
END_TEST

START_TEST(helpers_rotation_test1) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 0;
  prms.stats->falling_tetromino.coordinates.row = 5;
  prms.stats->falling_tetromino.coordinates.col = 2;
  placeTetromino(tetris, tetris->falling_tetromino);
  newFallingFigure(tetris);
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 0;
  prms.stats->falling_tetromino.coordinates.row = 6;
  prms.stats->falling_tetromino.coordinates.col = 0;
  placeTetromino(tetris, tetris->falling_tetromino);

  prms.stats->falling_tetromino.orient = 3;

  ck_assert_int_eq(checkRotationPossibility(
                       &prms, prms.stats->falling_tetromino.coordinates.col,
                       prms.stats->falling_tetromino.orient),
                   false);
}
END_TEST

START_TEST(helpers_rotation_test2) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 0;
  prms.stats->falling_tetromino.coordinates.row = 5;
  prms.stats->falling_tetromino.coordinates.col = 2;
  placeTetromino(tetris, tetris->falling_tetromino);
  newFallingFigure(tetris);
  prms.stats->falling_tetromino.type = 0;
  prms.stats->falling_tetromino.orient = 0;
  prms.stats->falling_tetromino.coordinates.row = 6;
  prms.stats->falling_tetromino.coordinates.col = 0;
  placeTetromino(tetris, tetris->falling_tetromino);

  prms.stats->falling_tetromino.orient = 3;

  ck_assert_int_eq(checkRotationPossibility(
                       &prms, prms.stats->falling_tetromino.coordinates.col,
                       prms.stats->falling_tetromino.orient),
                   false);

  prms.stats->falling_tetromino.orient = 1;

  ck_assert_int_eq(checkRotationPossibility(
                       &prms, prms.stats->falling_tetromino.coordinates.col,
                       prms.stats->falling_tetromino.orient),
                   false);
}
END_TEST

START_TEST(helpers_full_line_loop_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);

  for (int i = 0; i < BOARD_WIDTH; i++) {
    setCell(tetris, _i, i, 1);
  }
  ck_assert_int_eq(lineFull(tetris, _i), true);
}
END_TEST

START_TEST(helpers_full_line_loop_test1) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);

  for (int i = 1; i < BOARD_WIDTH; i++) {
    setCell(tetris, _i, i, 1);
  }
  ck_assert_int_eq(lineFull(tetris, _i - 1), false);
}
END_TEST

START_TEST(helpers_full_line_loop_test2) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);

  for (int i = 0; i < BOARD_WIDTH; i++) {
    setCell(tetris, _i, i, 1);
  }
  ck_assert_int_eq(lineFull(tetris, _i), true);
  shiftDown(tetris, _i + 1);
  ck_assert_int_eq(lineFull(tetris, _i + 1), true);
}
END_TEST

START_TEST(helpers_reset_data_test0) {
  game test = {0};
  game *tetris = &test;
  gameInit(tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_state state = 0;
  tetris_state *check = &state;
  params_t prms;
  prms.stats = tetris;
  prms.state = check;
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      setCell(tetris, i, j, 1);
    }
  }
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    ck_assert_int_eq(lineFull(tetris, i), true);
  }
  resetData(&prms);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    ck_assert_int_eq(lineFull(tetris, i), false);
  }
}
END_TEST

Suite *helpers_suite(void) {
  Suite *s;
  TCase *tc_main;

  s = suite_create("helpers tests");
  tc_main = tcase_create("main");

  tcase_add_test(tc_main, helpers_rotation_test0);
  tcase_add_test(tc_main, helpers_rotation_test1);
  tcase_add_test(tc_main, helpers_rotation_test2);
  tcase_add_test(tc_main, helpers_reset_data_test0);

  tcase_add_loop_test(tc_main, helpers_rotation_loop_test0, 0, NUM_TETROMINOES);
  tcase_add_loop_test(tc_main, helpers_rotation_loop_test1, 0, NUM_TETROMINOES);
  tcase_add_loop_test(tc_main, helpers_full_line_loop_test0, 0, BOARD_HEIGHT);
  tcase_add_loop_test(tc_main, helpers_full_line_loop_test1, 1, BOARD_HEIGHT);
  tcase_add_loop_test(tc_main, helpers_full_line_loop_test2, 0,
                      BOARD_HEIGHT - 1);

  suite_add_tcase(s, tc_main);
  return s;
}