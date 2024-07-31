#ifndef VIEW_HPP
#define VIEW_HPP

#include <ncurses.h>

#include <string>

#include "Controller.hpp"
// #include "Model.hpp"

#define ENTER_KEY 10
#define ESC_KEY 27

namespace s21 {

class View {
 public:
  View(int width = 20, int height = 20);
  ~View();
  void draw(const std::unique_ptr<BrickGame> &model);
  void drawGame(const std::unique_ptr<BrickGame> &model);
  void drawStartScreen(const std::unique_ptr<BrickGame> &model);
  void drawPauseScreen(const std::unique_ptr<BrickGame> &model);
  void drawGameOver(const std::unique_ptr<BrickGame> &model);
  void SignalProcessing();
  void MenuProcessing();
  void InitMenu();
  void ApplyChoice(int &choice);
  void DrawMenu(const std::vector<std::string> &options, int &menu_option);
  void StartSnakeGame();

 private:
  int key;
  Controller controller;
  int width, height;
  MenuState state;
  Signals signal;
  WINDOW *menuWin;
  WINDOW *startWin;
  WINDOW *pauseWin;
  WINDOW *gameOverWin;
  WINDOW *gameWin;
  WINDOW *sideBarWin;
};
}  // namespace s21
#endif  // VIEW_HPP