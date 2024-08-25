#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <memory>
#include <thread>

#include "Model.hpp"
#include "TetrisModel.hpp"

namespace s21 {
enum class MenuState { kMenu, kSnakeGame, kTetrisGame, kExitMenu };

class Controller {
 public:
  Controller();
  void SetGame(MenuState state);
  void getData(GameInfo_t& game);
  GameState GetCurrentGameState();
  std::unique_ptr<BrickGame>& GetModel();
  void GameProcessing(Signals);

 private:
  std::unique_ptr<BrickGame> model;
};

}  // namespace s21
#endif  // CONTROLLER_HPP