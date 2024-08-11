#include "../include/Controller.hpp"

namespace s21 {

Controller::Controller() : model(nullptr) {}

void Controller::setGame(MenuState state) {
  switch (state) {
    case MenuState::SNAKE_GAME:
      model = std::make_unique<SnakeModel>();
      break;
    case MenuState::TETRIS_GAME:
      model = std::make_unique<SnakeModel>();
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

// void Controller::run() {
//   // view.draw(model);

//   while (model.getCurrentState() != GameState::EXIT) {
//     Signals signal = view.SignalProcessing();
//     model.handleEvent(signal);
//     view.draw(model);

//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//   }
// }

}  // namespace s21