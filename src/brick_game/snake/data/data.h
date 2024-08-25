#ifndef CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_DATA_DATA_H_
#define CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_DATA_DATA_H_

namespace s21 {

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

}  // namespace s21

#endif  // CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_DATA_DATA_H_