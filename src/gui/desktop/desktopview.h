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
// #include <string>
// #include <vector>

#include "../../brick_game/controller/controller.h"

namespace s21 {

class DesktopView : public QOpenGLWidget {
 public:
  DesktopView();
  ~DesktopView();
  void initializeGL() override;
  void resizeGL(int, int) override;
  void paintGL() override;
  void keyPressEvent(QKeyEvent *) override;
  void setClearColor(const QColor &color);
  void setWindowTitle(const QString &title);

 private:
  void MemoryAllocation();
  void MemoryDeallocation();
  void updateGame();
  void draw(const GameInfo_t &game);
  void drawGame(const GameInfo_t &game);
  void drawOuterFrame();
  void drawSidebar(const GameInfo_t &game);
  void drawNextFigure(const GameInfo_t &game);
  void drawGameField(const GameInfo_t &game);
  void drawGameStateScreen(const GameState &state, const GameInfo_t &game);
  void InitColors();
  void SignalProcessing(QKeyEvent *);
  void MenuProcessing();
  void InitMenu();
  void ApplyChoice(int &);
  void DrawMenu(const std::vector<std::string> &options, int &menu_option);
  void StartTheGame();

  Controller controller;
  int screenWidth, screenHeight, blockSize, borderOffset;
  const int board_begin;
  Signals signal;
  MenuState state;
  QTimer *gameTimer;
  GameInfo_t currentGame;
  QPainter painter;
  GameState game_state;
  QMap<int, QColor> colorMap;
};

}  // namespace s21

#endif  // DESKTOPVIEW_H
