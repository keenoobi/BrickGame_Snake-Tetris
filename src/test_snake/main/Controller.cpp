#include "../include/Controller.hpp"

namespace s21 {

Controller::Controller(SnakeModel& model, SnakeView& view)
    : model(model), view(view) {
  // lastUpdateTime = GetCurrentTimeInMilliseconds();
}

void Controller::run() {
  // view.draw(model);

  while (model.getCurrentState() != GameState::EXIT) {
    Signals signal = view.handleInput();
    model.handleEvent(signal);
    view.draw(model);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

}  // namespace s21