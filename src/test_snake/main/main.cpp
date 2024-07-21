#include "../include/Controller.hpp"
#include "../include/Model.hpp"
#include "../include/View.hpp"

int main() {
  srand(time(0));
  int width = 20, height = 20;
  s21::SnakeModel model(width, height);
  s21::SnakeView view(width, height);
  s21::Controller controller(model, view);
  controller.run();
  return 0;
}