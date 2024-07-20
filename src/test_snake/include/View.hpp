#ifndef VIEW_HPP
#define VIEW_HPP

#include <ncurses.h>

#include "Model.hpp"

namespace s21 {

class SnakeView {
 public:
  SnakeView(int width, int height);
  ~SnakeView();
  void draw(const SnakeModel& model);
  void drawGameOver(const SnakeModel& model);

 private:
  int width, height;
  WINDOW* gameWin;
  WINDOW* sideBarWin;
};
}  // namespace s21
#endif  // VIEW_HPP