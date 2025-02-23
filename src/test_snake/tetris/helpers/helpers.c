#include "helpers.h"

bool checkRotationPossibility(params_t *prms, int col, int ori) {
  bool status = true;
  // checking right side
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  // checking left side
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
  // default check
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    prms->stats->falling_tetromino.coordinates.col = col;
    prms->stats->falling_tetromino.orient = ori;
    status = false;
  }
  return status;
}

bool lineFull(game *tetris, int row) {
  bool full = true;
  for (int j = 0; j < tetris->cols && full; j++) {
    if (getCell(tetris, row, j) == 0) full = false;
  }
  return full;
}

void shiftDown(game *tetris, int row) {
  for (int i = row - 1; i >= 0; i--) {
    for (int j = 0; j < tetris->cols; j++) {
      setCell(tetris, i + 1, j, getCell(tetris, i, j));
    }
  }
}

void resetData(params_t *prms) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      prms->stats->board[i][j] = 0;
    }
  }
  prms->stats->level = 0;
  prms->stats->score = 0;
}

int getCell(game *tetris, int row, int column) {
  return tetris->board[row][column];
}

void setCell(game *tetris, int row, int column, int value) {
  tetris->board[row][column] = value;
}

bool checkBounds(game *tetris, int row, int column) {
  return row >= 0 && column >= 0 && row < tetris->rows && column < tetris->cols;
}

void randomSortArray(int *arr) {
  int temp = 0, j = 0;
  for (int i = NUM_TETROMINOES - 1; i > 0; i--) {
    j = rand() % (i + 1);
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void gameInit(game *tetris, int rows, int cols) {
  tetris->rows = rows;
  tetris->cols = cols;
  tetris->level = 0;
  loadHighestScore(&tetris->record, RECORDS_FILE);
  tetris->tick_till_drop = kGravityLevel[tetris->level];
  srand(time(NULL));
  newFallingFigure(tetris);
}
