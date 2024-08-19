#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <memory>
#include <thread>

#include "Model.hpp"
#include "TetrisModel.hpp"
// #include "data.h"
// #include "View.hpp"
// #include "baseModel.hpp"

namespace s21 {
enum class MenuState { MENU, SNAKE_GAME, TETRIS_GAME, EXIT_MENU };

class Controller {
 public:
  Controller();
  void setGame(MenuState state);
  void getData(GameInfo_t& game);
  GameState GetCurrentGameState();
  std::unique_ptr<BrickGame>& GetModel();
  // void run();
  void GameProcessing(Signals);

 private:
  std::unique_ptr<BrickGame> model;
  // SnakeModel& model;
  // View& view;
};

}  // namespace s21
#endif  // CONTROLLER_HPP