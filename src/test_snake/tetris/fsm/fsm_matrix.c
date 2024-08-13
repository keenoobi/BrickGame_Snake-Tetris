#include "fsm.h"

action fsm_table[7][8] = {
    {NULL, NULL, NULL, NULL, NULL, exitstate, spawn, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {shifting, moveDown, moveRight, moveLeft, rotate, exitstate, NULL,
     pauseGame},
    {attaching, attaching, attaching, attaching, attaching, attaching,
     attaching, attaching},
    {gameOver, gameOver, gameOver, gameOver, gameOver, exitstate, start,
     gameOver},
    {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
     exitstate},
    {NULL, NULL, NULL, NULL, NULL, exitstate, NULL, pauseGame}};

void sigact(Signals_t signal, tetris_state *state, game *tetris) {
  params_t prms = {0};
  prms.stats = tetris;
  prms.state = state;

  action act = fsm_table[*state][signal];

  if (act) act(&prms);
}

void start(params_t *prms) {
  resetData(prms);
  *prms->state = SPAWN;
}

void exitstate(params_t *prms) { *prms->state = EXIT_STATE; }

void spawn(params_t *prms) {
  newFallingFigure(prms->stats);
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino))
    *prms->state = GAMEOVER;
  else {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = MOVING;
  }
}

void shifting(params_t *prms) {
  if (prms->stats->tick_till_drop-- <= 0) {
    prms->stats->tick_till_drop = kGravityLevel[prms->stats->level];
    moveDown(prms);
  }
}

void moveDown(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.row++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  } else {
    prms->stats->falling_tetromino.coordinates.row--;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = ATTACHING;
  }
}

void moveRight(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.col++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);

  } else {
    prms->stats->falling_tetromino.coordinates.col--;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  }
}

void moveLeft(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.col--;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  } else {
    prms->stats->falling_tetromino.coordinates.col++;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  }
}

void rotate(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  int tmp_col = prms->stats->falling_tetromino.coordinates.col;
  int tmp_ori = prms->stats->falling_tetromino.orient;

  if (prms->stats->falling_tetromino.type != 3 &&
      prms->stats->falling_tetromino.orient++ >= 3)
    prms->stats->falling_tetromino.orient = 0;

  checkRotationPossibility(prms, tmp_col, tmp_ori);
  placeTetromino(prms->stats, prms->stats->falling_tetromino);
}

void attaching(params_t *prms) {
  int lines_destroyed = 0;
  for (int row = prms->stats->rows - 1; row >= 0; row--) {
    if (lineFull(prms->stats, row)) {
      shiftDown(prms->stats, row);
      row++;
      lines_destroyed++;
    }
  }
  countPoints(prms, lines_destroyed);
  *prms->state = SPAWN;
}

void pauseGame(params_t *prms) {
  if (*prms->state != PAUSE) {
    *prms->state = PAUSE;
  } else {
    *prms->state = MOVING;
  }
}

void gameOver(params_t *prms) { *prms->state = GAMEOVER; }