#include "../include/Controller.hpp"

SnakeController::SnakeController(SnakeModel& model, SnakeView& view)
    : model(model), view(view) {
  lastUpdateTime = GetCurrentTimeInMilliseconds();
}

void SnakeController::run() {
  while (true) {
    if (model.isGameOver()) {
      view.drawGameOver(model);
      if (handleInput() == ENTER_KEY) {
        model.resetGame();
      }
    } else {
      long long currentTime = GetCurrentTimeInMilliseconds();
      int currentSpeed = model.isAccelerationOn() ? model.getAccelerationSpeed()
                                                  : model.getSpeed();
      if (currentTime - lastUpdateTime >= currentSpeed) {
        handleInput();
        model.moveSnake();
        view.draw(model);
        lastUpdateTime = currentTime;
      }
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int SnakeController::handleInput() {
  int ch = getch();
  if (ch != ERR) {
    switch (ch) {
      case KEY_UP:
        if (model.getCurrentDirection() != Direction::DOWN)
          model.setCurrentDirection(Direction::UP);
        break;
      case KEY_DOWN:
        if (model.getCurrentDirection() != Direction::UP)
          model.setCurrentDirection(Direction::DOWN);
        break;
      case KEY_LEFT:
        if (model.getCurrentDirection() != Direction::RIGHT)
          model.setCurrentDirection(Direction::LEFT);
        break;
      case KEY_RIGHT:
        if (model.getCurrentDirection() != Direction::LEFT)
          model.setCurrentDirection(Direction::RIGHT);
        break;
      case ' ':
        model.setAcceleration(true);
        break;
    }
    flushinp();
  } else {
    model.setAcceleration(false);
  }
  return ch;
}

long long SnakeController::GetCurrentTimeInMilliseconds() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch());
  return duration.count();
}