#include "test.h"

START_TEST(tetromino_fits_test0) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 0;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  ck_assert_int_eq(tetrominoFits(&tetris, block), true);
}
END_TEST

START_TEST(tetromino_fits_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  placeTetromino(&tetris, block);
  block.coordinates.row--;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
  block.coordinates.col--;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_fits_test2) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  placeTetromino(&tetris, block);
  block.coordinates.row++;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_fits_test3) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  placeTetromino(&tetris, block);
  block.coordinates.row += 2;
  ck_assert_int_eq(tetrominoFits(&tetris, block), true);
}
END_TEST

START_TEST(tetromino_fits_test4) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.row += 22;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_fits_test5) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.row -= 2;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_fits_test6) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 15;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.row -= 2;
  ck_assert_int_eq(tetrominoFits(&tetris, block), true);
}
END_TEST

START_TEST(tetromino_fits_test7) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.col += 2;
  ck_assert_int_eq(tetrominoFits(&tetris, block), true);
}
END_TEST

START_TEST(tetromino_fits_test8) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.col += 10;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_fits_test9) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris_block block = {0};
  block.type = 3;
  block.orient = 0;
  block.coordinates.row = 0;
  block.coordinates.col = BOARD_WIDTH / 2 - 2;
  block.coordinates.col -= 10;
  ck_assert_int_eq(tetrominoFits(&tetris, block), false);
}
END_TEST

START_TEST(tetromino_random_type_loop_test1) {
  tetris_block block = {0};
  block.type = randomTetromino();

  _ck_assert_int(block.type, <=, 6);
  _ck_assert_int(block.type, >=, 0);
}
END_TEST

START_TEST(tetromino_new_test1) {
  game tetris = {0};
  tetris.rows = 20;
  tetris.cols = 10;
  newFallingFigure(&tetris);
  ck_assert_int_eq(tetris.next_tetromino.orient, 0);
  ck_assert_int_eq(tetris.next_tetromino.coordinates.row, 0);
  ck_assert_int_eq(tetris.next_tetromino.coordinates.col, tetris.cols / 2 - 2);
}
END_TEST

START_TEST(tetromino_new_test2) {
  game tetris = {0};
  tetris.rows = 20;
  tetris.cols = 10;
  newFallingFigure(&tetris);
  newFallingFigure(&tetris);
  ck_assert_int_eq(tetris.falling_tetromino.orient, 0);
  ck_assert_int_eq(tetris.falling_tetromino.coordinates.row, 0);
  ck_assert_int_eq(tetris.falling_tetromino.coordinates.col,
                   tetris.cols / 2 - 2);

  ck_assert_int_eq(tetris.next_tetromino.orient, 0);
  ck_assert_int_eq(tetris.next_tetromino.coordinates.row, 0);
  ck_assert_int_eq(tetris.next_tetromino.coordinates.col, tetris.cols / 2 - 2);
}
END_TEST

START_TEST(tetromino_place_loop_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeTetromino(&tetris, tetris.falling_tetromino);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (tetris.board[i][j])
        ck_assert_int_eq(tetris.board[i][j], tetris.falling_tetromino.type + 1);
      else
        ck_assert_int_eq(tetris.board[i][j], 0);
    }
  }
}
END_TEST

START_TEST(tetromino_placeNext_loop_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeNextTetromino(&tetris, tetris.next_tetromino);
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (tetris.next_figure[i][j])
        ck_assert_int_eq(tetris.next_figure[i][j],
                         tetris.next_tetromino.type + 1);
      else
        ck_assert_int_eq(tetris.next_figure[i][j], 0);
    }
  }
}
END_TEST

START_TEST(tetromino_removeTetromino_loop_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeTetromino(&tetris, tetris.falling_tetromino);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (tetris.board[i][j])
        ck_assert_int_eq(tetris.board[i][j], tetris.falling_tetromino.type + 1);
      else
        ck_assert_int_eq(tetris.board[i][j], 0);
    }
  }

  removeTetromino(&tetris, tetris.falling_tetromino);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      ck_assert_int_eq(tetris.board[i][j], 0);
    }
  }
}
END_TEST

START_TEST(tetromino_removeNext_loop_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeNextTetromino(&tetris, tetris.next_tetromino);
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (tetris.next_figure[i][j])
        ck_assert_int_eq(tetris.next_figure[i][j],
                         tetris.next_tetromino.type + 1);
      else
        ck_assert_int_eq(tetris.next_figure[i][j], 0);
    }
  }
  removeNextTetromino(&tetris);
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      ck_assert_int_eq(tetris.board[i][j], 0);
    }
  }
}
END_TEST

Suite *tetromino_suite(void) {
  Suite *s;
  TCase *tc_main;

  s = suite_create("\n tetromino tests");
  tc_main = tcase_create("main");

  tcase_add_test(tc_main, tetromino_fits_test0);
  tcase_add_test(tc_main, tetromino_fits_test1);
  tcase_add_test(tc_main, tetromino_fits_test2);
  tcase_add_test(tc_main, tetromino_fits_test3);
  tcase_add_test(tc_main, tetromino_fits_test4);
  tcase_add_test(tc_main, tetromino_fits_test5);
  tcase_add_test(tc_main, tetromino_fits_test6);
  tcase_add_test(tc_main, tetromino_fits_test7);
  tcase_add_test(tc_main, tetromino_fits_test8);
  tcase_add_test(tc_main, tetromino_fits_test9);
  tcase_add_test(tc_main, tetromino_fits_test9);
  tcase_add_test(tc_main, tetromino_new_test1);
  tcase_add_test(tc_main, tetromino_new_test2);

  tcase_add_loop_test(tc_main, tetromino_random_type_loop_test1, 0, 50);
  tcase_add_loop_test(tc_main, tetromino_place_loop_test1, 0, 50);
  tcase_add_loop_test(tc_main, tetromino_placeNext_loop_test1, 0, 50);
  tcase_add_loop_test(tc_main, tetromino_removeTetromino_loop_test1, 0, 50);
  tcase_add_loop_test(tc_main, tetromino_removeNext_loop_test1, 0, 50);

  suite_add_tcase(s, tc_main);
  return s;
}