#ifndef CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_BASE_BASE_MODEL_H_
#define CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_BASE_BASE_MODEL_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../snake/data/data.h"

namespace s21 {
enum class Direction { kUp, kDown, kLeft, kRight };
enum class GameState { kStart, kPlaying, kGameOver, kPause, kExit };
enum class Signals { kNone, kUp, kDown, kRight, kLeft, kEsc, kEnter, kPause };

class BrickGame {
 public:
  virtual void GetData(GameInfo_t& game) const = 0;
  virtual GameState GetCurrentState() const = 0;
  virtual void HandleEvent(Signals) = 0;
  virtual ~BrickGame() = default;
};
}  // namespace s21

#endif  // CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_BASE_BASE_MODEL_H_