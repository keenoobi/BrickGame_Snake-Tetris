#include "../include/Controller.hpp"

namespace s21 {

SnakeController::SnakeController(SnakeModel& model, SnakeView& view)
    : model(model), view(view) {
  // lastUpdateTime = GetCurrentTimeInMilliseconds();
}

void SnakeController::run() {
  // view.draw(model);

  while (model.getCurrentState() != GameState::EXIT) {
    Signals signal = handleInput();
    // if (signal == Signals::PAUSE) printf("Pause1\n");
    model.handleEvent(signal);
    // if (signal == Signals::PAUSE) printf("Pause2\n");
    view.draw(model);
    // if (signal == Signals::PAUSE) printf("\nPause3\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // if (model.isGameOver()) {
  //   view.drawGameOver(model);
  //   if (handleInput() == ENTER_KEY) {
  //     model.resetGame();
  //   }
  // } else {
  //   long long currentTime = GetCurrentTimeInMilliseconds();
  //   int currentSpeed = model.isAccelerationOn() ?
  //   model.getAccelerationSpeed()
  //                                               : model.getSpeed();
  //   if (currentTime - lastUpdateTime >= currentSpeed) {
  //     handleInput();
  //     model.moveSnake();
  //     view.draw(model);
  //     lastUpdateTime = currentTime;
  //   }
  // }
}

Signals SnakeController::handleInput() {
  int key = getch();
  Signals sig = Signals::NONE;
  if (key != ERR) {
    switch (key) {
      case KEY_UP:
        sig = Signals::UP;
        break;
      case KEY_DOWN:
        sig = Signals::DOWN;
        break;
      case KEY_LEFT:
        sig = Signals::LEFT;
        break;
      case KEY_RIGHT:
        sig = Signals::RIGHT;
        break;
      case ENTER_KEY:
        sig = Signals::ENTER;
        break;
      case 'p':
        sig = Signals::PAUSE;
        break;
      case ESC_KEY:
        sig = Signals::ESC;
    }
  }
  return sig;
}

}  // namespace s21