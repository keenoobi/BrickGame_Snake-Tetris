#include "../include/Controller.hpp"
#include "../include/Model.hpp"
#include "../include/View.hpp"

int main() {
  SnakeModel model(10, 20);
  SnakeView view(10, 20);
  SnakeController controller(model, view);
  controller.run();
  return 0;
}