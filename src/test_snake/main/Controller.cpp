#include "../include/Controller.hpp"

namespace s21 {

Controller::Controller() : model(nullptr) {}

void Controller::SetGame(MenuState state) {
  switch (state) {
    case MenuState::kSnakeGame:
      model = std::make_unique<SnakeModel>();
      break;
    case MenuState::kTetrisGame:
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
GameState Controller::GetCurrentGameState() { return model->GetCurrentState(); }
void Controller::GameProcessing(Signals signal) { model->HandleEvent(signal); }

}  // namespace s21
