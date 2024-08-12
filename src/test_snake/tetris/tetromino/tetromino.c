#include "../tetris.h"

bool tetrominoFits(game *tetris, tetris_block block) {
  bool fits = true;
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = kTetrisFigure[block.type][block.orient][i];
    int r = block.coordinates.row + cell.row;
    int c = block.coordinates.col + cell.col;
    if (!checkBounds(tetris, r, c) || getCell(tetris, r, c) != 0) {
      fits = false;
    }
  }
  return fits;
}

int randomTetromino() {
  static int tetrominos_array[NUM_TETROMINOES] = {0};
  static int n = -1;
  n++;
  if (n >= NUM_TETROMINOES) n = 0;
  if (n == 0) {
    for (int i = 0; i < NUM_TETROMINOES; i++) {
      tetrominos_array[i] = i;
    }
    randomSortArray(tetrominos_array);
  }
  // printw(" n%d p%d ", n, tetrominos_array[n]);
  return tetrominos_array[n];
}

void newFallingFigure(game *tetris) {
  tetris->falling_tetromino = tetris->next_tetromino;
  tetris->next_tetromino.type = randomTetromino();
  tetris->next_tetromino.orient = 0;
  tetris->next_tetromino.coordinates.row = 0;
  tetris->next_tetromino.coordinates.col = tetris->cols / 2 - 2;
  removeNextTetromino(tetris);
  placeNextTetromino(tetris, tetris->next_tetromino);
}

void placeTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = kTetrisFigure[piece.type][piece.orient][i];
    setCell(tetris, piece.coordinates.row + cell.row,
            piece.coordinates.col + cell.col, piece.type + 1);
  }
}

void placeNextTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = kTetrisFigure[piece.type][piece.orient][i];
    tetris->next_figure[cell.row][cell.col] = piece.type + 1;
  }
}

void removeNextTetromino(game *tetris) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      tetris->next_figure[i][j] = 0;
    }
  }
}

void removeTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < 4; i++) {
    tetris_location cell = kTetrisFigure[piece.type][piece.orient][i];
    setCell(tetris, piece.coordinates.row + cell.row,
            piece.coordinates.col + cell.col, 0);
  }
}