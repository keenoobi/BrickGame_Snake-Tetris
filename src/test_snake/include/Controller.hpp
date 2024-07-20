#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <chrono>
#include <thread>

#include "Model.hpp"
#include "View.hpp"

#define ENTER_KEY 10

namespace s21 {

class SnakeController {
 public:
  SnakeController(SnakeModel& model, SnakeView& view);
  void run();

 private:
  int handleInput();
  static long long GetCurrentTimeInMilliseconds();

  SnakeModel& model;
  SnakeView& view;
  long long lastUpdateTime;
};

}  // namespace s21
#endif  // CONTROLLER_HPP