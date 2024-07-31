#include "../include/View.hpp"

namespace s21 {

View::View(int width, int height)
    : width(width),
      height(height),
      signal(Signals::NONE),
      state(MenuState::MENU),
      key(0) {
  controller = {};
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  notimeout(stdscr, TRUE);
  menuWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
  gameWin = newwin(height + 2, width + 2, 2, 2);
  sideBarWin = newwin(height + 2, width - 5, 2, width + 4);
  startWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
  pauseWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
  gameOverWin = newwin(height + 2, height + 2 + width - 5, 2, 2);
}

View::~View() {
  delwin(startWin);
  delwin(menuWin);
  delwin(pauseWin);
  delwin(gameOverWin);
  delwin(gameWin);
  delwin(sideBarWin);
  endwin();
}

void View::MenuProcessing() {
  while (state != MenuState::EXIT_MENU) {
    switch (state) {
      case MenuState::MENU:
        InitMenu();
        break;
      case MenuState::SNAKE_GAME:
        StartSnakeGame();
        break;
      case MenuState::TETRIS_GAME:
        // StartTetrisGame();
        break;
      case MenuState::EXIT_MENU:
        // InitExit();
        break;
      default:
        break;
    }
  }
}

void View::StartSnakeGame() {
  controller.setGame(state);
  while (controller.GetCurrentGameState() != GameState::EXIT) {
    SignalProcessing();
    controller.GameProcessing(signal);
    draw(controller.GetModel());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  state = MenuState::MENU;
}

void View::InitMenu() {
  static int menu_option = 0;
  bool enter = false;
  std::vector<std::string> options = {"Tetris", "Snake", "Exit"};

  SignalProcessing();

  switch (signal) {
    case Signals::UP:
      if (menu_option > 0) menu_option--;
      break;
    case Signals::DOWN:
      if (menu_option < 2) menu_option++;
      break;
    case Signals::ENTER:
      enter = true;
      ApplyChoice(menu_option);
      break;
  }

  if (!enter) DrawMenu(options, menu_option);
}

void View::ApplyChoice(int &choice) {
  switch (choice) {
    case 0:
      state = MenuState::TETRIS_GAME;
      break;
    case 1:
      state = MenuState::SNAKE_GAME;
      break;
    case 2:
      state = MenuState::EXIT_MENU;
      break;
    default:
      break;
  }
}

void View::DrawMenu(const std::vector<std::string> &options, int &menu_option) {
  box(menuWin, 0, 0);
  for (int i = 0; i < 3; i++) {
    mvwprintw(menuWin, i + 1, 2, "%s", options[i].c_str());
    if (i == menu_option) {
      wattron(menuWin, A_REVERSE);
      mvwprintw(menuWin, i + 1, 2, "%s", options[i].c_str());
      wattroff(menuWin, A_REVERSE);
    }
  }
  wrefresh(menuWin);
}

void View::SignalProcessing() {
  key = getch();
  switch (key) {
    case KEY_UP:
      signal = Signals::UP;
      break;
    case KEY_DOWN:
      signal = Signals::DOWN;
      break;
    case KEY_LEFT:
      signal = Signals::LEFT;
      break;
    case KEY_RIGHT:
      signal = Signals::RIGHT;
      break;
    case ENTER_KEY:
      signal = Signals::ENTER;
      break;
    case 'p':
      signal = Signals::PAUSE;
      break;
    case ESC_KEY:
      signal = Signals::ESC;
      break;
    default:
      signal = Signals::NONE;
      break;
  }
}

void View::drawStartScreen(const std::unique_ptr<BrickGame> &model) {
  if (model->getCurrentState() == GameState::START) {
    box(startWin, 0, 0);
    mvwprintw(startWin, 10, 8, "Press Enter to start");
    wrefresh(startWin);
  }
}

void View::drawPauseScreen(const std::unique_ptr<BrickGame> &model) {
  if (model->getCurrentState() == GameState::PAUSE) {
    box(pauseWin, 0, 0);
    mvwprintw(pauseWin, 10, 5, "Game is paused");
    mvwprintw(pauseWin, 10 + 2, 5, "Press P to resume");
    wrefresh(pauseWin);
  }
}

void View::drawGame(const std::unique_ptr<BrickGame> &model) {
  if (model->getCurrentState() != GameState::PLAYING) return;
  wclear(gameWin);
  wclear(sideBarWin);
  box(gameWin, 0, 0);     // Draw a border around the window
  box(sideBarWin, 0, 0);  // Draw a border around the window

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (model->getGameBoard()[i][j]) {
        char symbol = (model->getGameBoard()[i][j] == 1)   ? 'O'
                      : (model->getGameBoard()[i][j] == 2) ? '*'
                                                           : ' ';
        mvwprintw(gameWin, i + 1, j + 1, "%c", symbol);
      }
    }
  }

  // mvwprintw(sideBarWin, 2, 2, "Score: %d", model.getScore());
  // mvwprintw(sideBarWin, 4, 2, "Level: %d", model.getLevel());
  // mvwprintw(sideBarWin, 6, 2, "Speed: %d", model.getSpeed());
  wrefresh(gameWin);
  wrefresh(sideBarWin);
}

void View::draw(const std::unique_ptr<BrickGame> &model) {
  switch (model->getCurrentState()) {
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

void View::drawGameOver(const std::unique_ptr<BrickGame> &model) {
  box(gameOverWin, 0, 0);
  mvwprintw(gameOverWin, 8, 12, "Game Over!");
  // mvwprintw(gameOverWin, 10, 8, "Score: %d  Level: %d", model.getScore(),
  //           model.getLevel());
  mvwprintw(gameOverWin, 12, 5, "Start over? (Press Enter)");
  wrefresh(gameOverWin);
}

}  // namespace s21