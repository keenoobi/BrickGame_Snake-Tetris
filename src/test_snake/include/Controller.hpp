#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <thread>

#include "Model.hpp"
#include "View.hpp"

namespace s21 {

class Controller {
 public:
  Controller(SnakeModel& model, SnakeView& view);
  void run();

 private:
  SnakeModel& model;
  SnakeView& view;
};

}  // namespace s21
#endif  // CONTROLLER_HPP