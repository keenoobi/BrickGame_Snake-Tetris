#ifndef CPP3_BRICKGAME_2_0_1_SRC_GUI_CLI_VIEW_H_
#define CPP3_BRICKGAME_2_0_1_SRC_GUI_CLI_VIEW_H_

#include <ncurses.h>

#include <string>

#include "../../brick_game/controller/controller.h"

namespace s21 {

class View {
 public:
  View(int width = 20, int height = 20);
  ~View();
  void MenuProcessing();

 private:
  void NcursesInit();
  void WindowsInit();
  void InitColors();
  void MemoryAllocation();
  void MemoryDeallocation();
  void Draw(const GameInfo_t &game);
  void DrawGame(const GameInfo_t &game);
  void DisplayNextFigure(const GameInfo_t &game);
  void DrawStartScreen(const GameState &state);
  void DrawPauseScreen(const GameState &state);
  void DrawGameOver(const GameInfo_t &game);
  void SignalProcessing();
  void InitMenu();
  void ApplyChoice(int &choice);
  void DrawMenu(const std::vector<std::string> &options, int &menu_option);
  void StartTheGame();

 private:
  Controller controller_;
  int width_, height_;
  Signals signal_;
  MenuState menu_state_;
  int key_;
  GameInfo_t game_;
  WINDOW *menu_win_;
  WINDOW *start_win_;
  WINDOW *pause_win_;
  WINDOW *game_over_win_;
  WINDOW *game_win_;
  WINDOW *sidebar_win_;
};
}  // namespace s21
#endif  // CPP3_BRICKGAME_2_0_1_SRC_GUI_CLI_VIEW_H_