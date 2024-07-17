#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>

#include <vector>

#include "Model.hpp"

#define WIN_INIT(time)       \
  {                          \
    initscr();               \
    noecho();                \
    cbreak();                \
    nodelay(stdscr, true);   \
    notimeout(stdscr, TRUE); \
    curs_set(0);             \
    keypad(stdscr, TRUE);    \
    timeout(time);           \
  }

class SnakeView {
 public:
  SnakeView(int width, int height);
  ~SnakeView();
  void draw(const SnakeModel& model);
  void drawGameOver(const SnakeModel& model);

 private:
  int width, height;
  WINDOW* gameWin;
};

#endif  // VIEW_H