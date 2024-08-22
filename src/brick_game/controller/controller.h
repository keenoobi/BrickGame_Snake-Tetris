#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <memory>
#include <thread>

#include "../snake/backend/snake_model.h"
#include "../tetris/tetris_model.h"

namespace s21 {
enum class MenuState { MENU, SNAKE_GAME, TETRIS_GAME, EXIT_MENU };

class Controller {
 public:
  Controller();
  void setGame(MenuState state);
  void getData(GameInfo_t& game);
  GameState GetCurrentGameState();
  std::unique_ptr<BrickGame>& GetModel();
  void GameProcessing(Signals);

 private:
  std::unique_ptr<BrickGame> model;
};

}  // namespace s21
#endif  // CONTROLLER_HPP