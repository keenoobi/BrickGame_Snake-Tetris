#include "../include/Controller.hpp"
#include "../include/Model.hpp"
#include "../include/View.hpp"

int main() {
  srand(time(0));
  s21::View view;

  view.MenuProcessing();
  // view.SignalProcessing();

  // int width = 20, height = 20;
  // s21::SnakeModel model(width, height);
  // s21::View view(width, height);
  s21::Controller controller;
  // controller.run();
  return 0;
}