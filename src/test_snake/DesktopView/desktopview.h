#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H

#include "../include/Controller.hpp"

#include <QOpenGLWidget>
#include <QColor>
#include <QPainter>
#include <QTimer>
#include <vector>
#include <string>


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

    void SignalProcessing(QKeyEvent *);
    void MenuProcessing();
    void InitMenu();
    void ApplyChoice(int &);
    void DrawMenu(const std::vector<std::string> &options, int &menu_option);
    void StartSnakeGame();


private:
    void MemoryAllocation();
    void MemoryDeallocation();
    void updateGame();
    void draw(const GameInfo_t &game);
    void drawGame(const GameInfo_t &game);
    void drawStartScreen(const GameState &state);
    void drawPauseScreen(const GameState &state);
    void drawGameOver(const GameInfo_t &game);

    Controller controller;
    int width, height;
    Signals signal;
    MenuState state;
    QTimer *gameTimer;
    GameInfo_t game;
    QPainter painter;
    GameState game_state;    // Общий QPainter
    // Добавьте другие необходимые переменные для игры
};

}

#endif // DESKTOPVIEW_H
