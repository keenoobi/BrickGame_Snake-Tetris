#include "api.h"

GameInfo_t updateCurrentState(game *tetris, GameInfo_t *graphics) {
  GameInfo_t *data = graphics;
  TRANSFER_DATA(BOARD_HEIGHT, BOARD_WIDTH, data->field, tetris->board);
  TRANSFER_DATA(TETROMINO_SIZE, TETROMINO_SIZE, data->next,
                tetris->next_figure);
  data->level = tetris->level;
  data->score = tetris->score;
  data->high_score = tetris->record;
  return *data;
}