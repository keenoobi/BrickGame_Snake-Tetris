#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H

#include <QColor>
#include <QFont>
#include <QKeyEvent>
#include <QMap>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPen>
#include <QTextOption>
#include <QTimer>
#include <iostream>
#include <string>
#include <vector>

#include "../include/Controller.hpp"

namespace s21 {

class DesktopView : public QOpenGLWidget {
 public:
  DesktopView();
  ~DesktopView();
  void initializeGL() override;
  void resizeGL(int, int) override;
  void paintGL() override;
  void keyPressEvent(QKeyEvent *) override;
  void SetClearColor(const QColor &color);
  void SetWindowTitle(const QString &title);

 private:
  void MemoryAllocation();
  void MemoryDeallocation();
  void UpdateGame();
  void Draw(const GameInfo_t &game);
  void DrawGame(const GameInfo_t &game);
  void drawOuterFrame();
  void DrawSidebar(const GameInfo_t &game);
  void DrawNextFigure(const GameInfo_t &game);
  void DrawGameField(const GameInfo_t &game);
  void DrawGameStateScreen(const GameState &state, const GameInfo_t &game);
  void InitColors();
  void SignalProcessing(QKeyEvent *);
  void MenuProcessing();
  void InitMenu();
  void ApplyChoice(int &);
  void DrawMenu(const std::vector<std::string> &options, int &menu_option);
  void StartTheGame();

  Controller controller_;
  int screen_width_, screen_height_, block_size_, border_offset_;
  const int board_begin_;
  Signals signal_;
  MenuState menu_state_;
  QTimer *game_timer_;
  GameInfo_t current_game_;
  QPainter painter_;
  GameState game_state_;
  QMap<int, QColor> color_map_;
};

}  // namespace s21

#endif  // DESKTOPVIEW_H
