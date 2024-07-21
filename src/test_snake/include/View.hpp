#ifndef VIEW_HPP
#define VIEW_HPP

#include <ncurses.h>

#include "Model.hpp"

#define ENTER_KEY 10
#define ESC_KEY 27

namespace s21 {

class SnakeView {
 public:
  SnakeView(int width, int height);
  ~SnakeView();
  void draw(const SnakeModel& model);
  void drawGame(const SnakeModel& model);
  void drawStartScreen(const SnakeModel& model);
  void drawPauseScreen(const SnakeModel& model);
  void drawGameOver(const SnakeModel& model);
  Signals handleInput();

 private:
  int width, height;
  WINDOW* startWin;
  WINDOW* pauseWin;
  WINDOW* gameOverWin;
  WINDOW* gameWin;
  WINDOW* sideBarWin;
};
}  // namespace s21
#endif  // VIEW_HPP