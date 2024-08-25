#include "view.h"

namespace s21 {

View::View(int width, int height)
    : width_(width),
      height_(height),
      signal_(Signals::kNone),
      menu_state_(MenuState::kMenu),
      key_(0) {
  game_ = {};
  controller_ = {};
  NcursesInit();
  WindowsInit();
  InitColors();
  MemoryAllocation();
}

void View::NcursesInit() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  notimeout(stdscr, TRUE);
}

void View::WindowsInit() {
  menu_win_ = newwin(height_ + 2, height_ + 2 + width_ - 5, 2, 2);
  game_win_ = newwin(height_ + 2, width_ + 2, 2, 2);
  sidebar_win_ = newwin(height_ + 2, width_ - 5, 2, width_ + 4);
  start_win_ = newwin(height_ + 2, height_ + 2 + width_ - 5, 2, 2);
  pause_win_ = newwin(height_ + 2, height_ + 2 + width_ - 5, 2, 2);
  game_over_win_ = newwin(height_ + 2, height_ + 2 + width_ - 5, 2, 2);
}

void View::InitColors(void) {
  start_color();

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_RED, COLOR_BLACK);
}

void View::MemoryAllocation() {
  game_.field = new int *[height_]();
  for (int i = 0; i < height_; ++i) {
    game_.field[i] = new int[width_]();
  }
  game_.next = new int *[4]();
  for (int i = 0; i < 4; ++i) {
    game_.next[i] = new int[4]();
  }
}

void View::MemoryDeallocation() {
  for (int i = 0; i < height_; ++i) {
    delete[] game_.field[i];
  }
  delete[] game_.field;
  for (int i = 0; i < 4; ++i) {
    delete[] game_.next[i];
  }
  delete[] game_.next;
}

View::~View() {
  delwin(start_win_);
  delwin(menu_win_);
  delwin(pause_win_);
  delwin(game_over_win_);
  delwin(game_win_);
  delwin(sidebar_win_);
  MemoryDeallocation();
}

void View::MenuProcessing() {
  while (menu_state_ != MenuState::kExitMenu) {
    SignalProcessing();
    switch (menu_state_) {
      case MenuState::kMenu:
        InitMenu();
        break;
      case MenuState::kSnakeGame:
        StartTheGame();
        break;
      case MenuState::kTetrisGame:
        StartTheGame();
        break;
      case MenuState::kExitMenu:
        // InitExit();
        break;
      default:
        break;
    }
  }
}

void View::StartTheGame() {
  if (controller_.GetCurrentGameState() != GameState::kExit) {
    controller_.GameProcessing(signal_);
    controller_.GetData(game_);
    Draw(game_);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } else {
    wclear(game_win_);
    wclear(sidebar_win_);
    menu_state_ = MenuState::kMenu;
  }
}

void View::InitMenu() {
  static int menu_option = 0;
  bool enter = false;
  std::vector<std::string> options = {"Tetris", "Snake", "Exit"};

  switch (signal_) {
    case Signals::kUp:
      if (menu_option > 0) menu_option--;
      break;
    case Signals::kDown:
      if (menu_option < 2) menu_option++;
      break;
    case Signals::kEnter:
      enter = true;
      ApplyChoice(menu_option);
      break;
    default:
      break;
  }

  if (!enter) DrawMenu(options, menu_option);
}

void View::ApplyChoice(int &choice) {
  switch (choice) {
    case 0:
      menu_state_ = MenuState::kTetrisGame;
      controller_.SetGame(menu_state_);
      break;
    case 1:
      menu_state_ = MenuState::kSnakeGame;
      controller_.SetGame(menu_state_);
      break;
    case 2:
      menu_state_ = MenuState::kExitMenu;
      break;
    default:
      break;
  }
}

void View::DrawMenu(const std::vector<std::string> &options, int &menu_option) {
  box(menu_win_, 0, 0);
  for (int i = 0; i < 3; i++) {
    mvwprintw(menu_win_, i + 1, 2, "%s", options[i].c_str());
    if (i == menu_option) {
      wattron(menu_win_, A_REVERSE);
      mvwprintw(menu_win_, i + 1, 2, "%s", options[i].c_str());
      wattroff(menu_win_, A_REVERSE);
    }
  }
  wrefresh(menu_win_);
}

void View::SignalProcessing() {
  key_ = getch();
  switch (key_) {
    case KEY_UP:
      signal_ = Signals::kUp;
      break;
    case KEY_DOWN:
      signal_ = Signals::kDown;
      break;
    case KEY_LEFT:
      signal_ = Signals::kLeft;
      break;
    case KEY_RIGHT:
      signal_ = Signals::kRight;
      break;
    case ENTER_KEY:
      signal_ = Signals::kEnter;
      break;
    case 'p':
      signal_ = Signals::kPause;
      break;
    case ESC_KEY:
      signal_ = Signals::kEsc;
      break;
    default:
      signal_ = Signals::kNone;
      break;
  }
}

void View::DrawStartScreen(const GameState &state) {
  if (state == GameState::kStart) {
    box(start_win_, 0, 0);
    mvwprintw(start_win_, 10, 8, "Press Enter to start");
    wrefresh(start_win_);
  }
}

void View::DrawPauseScreen(const GameState &state) {
  if (state == GameState::kPause) {
    box(pause_win_, 0, 0);
    mvwprintw(pause_win_, 10, 5, "Game is paused");
    mvwprintw(pause_win_, 10 + 2, 5, "Press P to resume");
    wrefresh(pause_win_);
  }
}

void View::DrawGame(const GameInfo_t &game) {
  box(game_win_, 0, 0);     // Draw a border around the window
  box(sidebar_win_, 0, 0);  // Draw a border around the window

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (game.field[i][j]) {
        ADD_BLOCK(game_win_, i, j, game.field[i][j]);
      } else {
        ADD_EMPTY(game_win_, i, j);
      }
    }
  }

  mvwprintw(sidebar_win_, 2, 2, "Score: %d", game.score);
  mvwprintw(sidebar_win_, 4, 2, "Level: %d", game.level);
  mvwprintw(sidebar_win_, 6, 2, "Record: %d", game.high_score);
  if (menu_state_ == MenuState::kSnakeGame)
    mvwprintw(sidebar_win_, 8, 2, "Speed: %d", 300 - game.speed);
  DisplayNextFigure(game);
  wrefresh(game_win_);
  wrefresh(sidebar_win_);
}

void View::DisplayNextFigure(const GameInfo_t &game) {
  if (menu_state_ != MenuState::kTetrisGame) return;

  box(sidebar_win_, 0, 0);

  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (game.next[i][j]) {
        ADD_BLOCK(sidebar_win_, i + 11, j + 1, game.next[i][j]);
      } else {
        ADD_EMPTY(sidebar_win_, i + 11, j + 1);
      }
    }
  }
}

void View::Draw(const GameInfo_t &game) {
  GameState state = controller_.GetCurrentGameState();
  switch (state) {
    case GameState::kStart:
      DrawStartScreen(state);
      break;
    case GameState::kPlaying:
      DrawGame(game);
      break;
    case GameState::kPause:
      DrawPauseScreen(state);
      break;
    case GameState::kGameOver:
      DrawGameOver(game);
      break;
    default:
      break;
  }
}

void View::DrawGameOver(const GameInfo_t &game) {
  wclear(game_win_);
  wclear(sidebar_win_);
  box(game_over_win_, 0, 0);

  if (game.score < 200)
    mvwprintw(game_over_win_, 8, 12, "Game Over!");
  else
    mvwprintw(game_over_win_, 8, 14, "YOU WON!");

  mvwprintw(game_over_win_, 10, 8, "Score: %d  Level: %d", game.score,
            game.level);
  mvwprintw(game_over_win_, 12, 5, "Start over? (Press Enter)");
  wrefresh(game_over_win_);
}

}  // namespace s21