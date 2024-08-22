#ifndef VIEW_HPP
#define VIEW_HPP

#include <ncurses.h>

#include <string>

#include "../../brick_game/controller/controller.h"

// #define ENTER_KEY 10
// #define ESC_KEY 27

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
  void draw(const GameInfo_t &game);
  void drawGame(const GameInfo_t &game);
  void displayNextFigure(const GameInfo_t &game);
  void drawStartScreen(const GameState &state);
  void drawPauseScreen(const GameState &state);
  void drawGameOver(const GameInfo_t &game);
  void SignalProcessing();
  void InitMenu();
  void ApplyChoice(int &choice);
  void DrawMenu(const std::vector<std::string> &options, int &menu_option);
  void StartTheGame();

 private:
  Controller controller;
  int width, height;
  Signals signal;
  MenuState state;
  int key;
  GameInfo_t game;
  WINDOW *menuWin;
  WINDOW *startWin;
  WINDOW *pauseWin;
  WINDOW *gameOverWin;
  WINDOW *gameWin;
  WINDOW *sideBarWin;
};
}  // namespace s21
#endif  // VIEW_HPP