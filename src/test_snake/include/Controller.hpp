#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <thread>

#include "Model.hpp"
#include "View.hpp"

#define ENTER_KEY 10
#define ESC_KEY 27

namespace s21 {

class SnakeController {
 public:
  SnakeController(SnakeModel& model, SnakeView& view);
  void run();

 private:
  Signals handleInput();

  SnakeModel& model;
  SnakeView& view;
};

}  // namespace s21
#endif  // CONTROLLER_HPP