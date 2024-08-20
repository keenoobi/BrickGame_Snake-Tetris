#include "../include/Controller.hpp"

namespace s21 {

Controller::Controller() : model(nullptr) {}

void Controller::setGame(MenuState state) {
  switch (state) {
    case MenuState::SNAKE_GAME:
      model = std::make_unique<SnakeModel>();
      break;
    case MenuState::TETRIS_GAME:
      model = std::make_unique<TetrisModel>();
      break;
    default:
      break;
  }
}

void Controller::getData(GameInfo_t& game) {
  if (model != nullptr) {
    model->GetData(game);
  }
}

std::unique_ptr<BrickGame>& Controller::GetModel() { return model; }
GameState Controller::GetCurrentGameState() { return model->getCurrentState(); }
void Controller::GameProcessing(Signals signal) { model->handleEvent(signal); }

}  // namespace s21
