#include "../include/Controller.hpp"
#include "../include/Model.hpp"
#include "../include/View.hpp"

int main() {
  srand(time(0));
  int width = 20, height = 20;
  SnakeModel model(width, height);
  SnakeView view(width, height);
  SnakeController controller(model, view);
  controller.run();
  return 0;
}