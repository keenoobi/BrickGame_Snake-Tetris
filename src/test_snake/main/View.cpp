#include "../include/View.hpp"

SnakeView::SnakeView(int width, int height) : width(width), height(height) {
  WIN_INIT(1);
  gameWin =
      newwin(height + 2, width + 2, (LINES - height) / 2, (COLS - width) / 2);
}

SnakeView::~SnakeView() {
  delwin(gameWin);
  endwin();
}

void SnakeView::draw(const SnakeModel& model) {
  wclear(gameWin);
  box(gameWin, 0, 0);  // Draw a border around the window

  for (const auto& part : model.getSnake()) {
    mvwprintw(gameWin, part.y + 1, part.x + 1, "O");
  }

  mvwprintw(gameWin, model.getFood().y + 1, model.getFood().x + 1, "*");

  mvwprintw(gameWin, height + 1, 1, "Score: %d  Level: %d", model.getScore(),
            model.getLevel());
  wrefresh(gameWin);
}

void SnakeView::drawGameOver(const SnakeModel& model) {
  wclear(gameWin);
  wprintw(gameWin, "Game Over! Score: %d  Level: %d\n", model.getScore(),
          model.getLevel());
  wprintw(gameWin, "Start over? (Press Enter)\n");
  wrefresh(gameWin);
}