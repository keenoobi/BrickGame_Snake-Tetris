#include "../include/View.hpp"

namespace s21 {

SnakeView::SnakeView(int width, int height) : width(width), height(height) {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, true);
  notimeout(stdscr, TRUE);
  gameWin = newwin(height + 2, width + 2, 2, 2);
  sideBarWin = newwin(height + 2, width - 5, 2, width + 4);
  startWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
  pauseWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
  gameOverWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
}

SnakeView::~SnakeView() {
  delwin(startWin);
  delwin(pauseWin);
  delwin(gameOverWin);
  delwin(gameWin);
  delwin(sideBarWin);
  endwin();
}

Signals SnakeView::handleInput() {
  int key = getch();
  Signals sig = Signals::NONE;
  if (key != ERR) {
    switch (key) {
      case KEY_UP:
        sig = Signals::UP;
        break;
      case KEY_DOWN:
        sig = Signals::DOWN;
        break;
      case KEY_LEFT:
        sig = Signals::LEFT;
        break;
      case KEY_RIGHT:
        sig = Signals::RIGHT;
        break;
      case ENTER_KEY:
        sig = Signals::ENTER;
        break;
      case 'p':
        sig = Signals::PAUSE;
        break;
      case ESC_KEY:
        sig = Signals::ESC;
        break;
    }
  }
  return sig;
}

void SnakeView::drawStartScreen(const SnakeModel& model) {
  if (model.getCurrentState() == GameState::START) {
    box(startWin, 0, 0);
    mvwprintw(startWin, 10, 8, "Press Enter to start");
    wrefresh(startWin);
  }
}

void SnakeView::drawPauseScreen(const SnakeModel& model) {
  if (model.getCurrentState() == GameState::PAUSE) {
    box(pauseWin, 0, 0);
    mvwprintw(pauseWin, 10, 5, "Game is paused");
    mvwprintw(pauseWin, 10 + 2, 5, "Press P to resume");
    wrefresh(pauseWin);
  }
}

void SnakeView::drawGame(const SnakeModel& model) {
  if (model.getCurrentState() != GameState::PLAYING) return;
  wclear(gameWin);
  wclear(sideBarWin);
  box(gameWin, 0, 0);     // Draw a border around the window
  box(sideBarWin, 0, 0);  // Draw a border around the window

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (model.getGameBoard()[i][j]) {
        char symbol = (model.getGameBoard()[i][j] == 1)   ? 'O'
                      : (model.getGameBoard()[i][j] == 2) ? '*'
                                                          : ' ';
        mvwprintw(gameWin, i + 1, j + 1, "%c", symbol);
      }
    }
  }

  mvwprintw(sideBarWin, 2, 2, "Score: %d", model.getScore());
  mvwprintw(sideBarWin, 4, 2, "Level: %d", model.getLevel());
  mvwprintw(sideBarWin, 6, 2, "Speed: %d", model.getSpeed());
  wrefresh(gameWin);
  wrefresh(sideBarWin);
}

void SnakeView::draw(const SnakeModel& model) {
  switch (model.getCurrentState()) {
    case GameState::START:
      drawStartScreen(model);
      break;
    case GameState::PLAYING:
      drawGame(model);
      break;
    case GameState::PAUSE:
      drawPauseScreen(model);
      break;
    case GameState::GAMEOVER:
      drawGameOver(model);
      break;
    default:
      break;
  }
}

void SnakeView::drawGameOver(const SnakeModel& model) {
  box(gameOverWin, 0, 0);
  mvwprintw(gameOverWin, 8, 12, "Game Over!");
  mvwprintw(gameOverWin, 10, 8, "Score: %d  Level: %d", model.getScore(),
            model.getLevel());
  mvwprintw(gameOverWin, 12, 5, "Start over? (Press Enter)");
  wrefresh(gameOverWin);
}

}  // namespace s21