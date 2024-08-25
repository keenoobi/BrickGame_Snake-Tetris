#ifndef CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_CONTROLLER_CONTROLLER_H_
#define CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_CONTROLLER_CONTROLLER_H_

#include <memory>
#include <thread>

#include "../snake/backend/snake_model.h"
#include "../tetris/tetris_model.h"

namespace s21 {
enum class MenuState { kMenu, kSnakeGame, kTetrisGame, kExitMenu };

class Controller {
 public:
  Controller();
  void SetGame(MenuState state);
  void GetData(GameInfo_t& game);
  GameState GetCurrentGameState();
  std::unique_ptr<BrickGame>& GetModel();
  void GameProcessing(Signals);

 private:
  std::unique_ptr<BrickGame> model;
};

}  // namespace s21
#endif  // CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_CONTROLLER_CONTROLLER_H_