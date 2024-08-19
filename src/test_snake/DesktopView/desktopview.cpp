#include "desktopview.h"
#include <QKeyEvent>
#include <QPainter>
#include <QFont>
#include <QTimer>
#include <iostream>

namespace s21 {

DesktopView::DesktopView()
    : width(800),
    height(600),
    signal(Signals::NONE),
    state(MenuState::MENU),
    gameTimer(new QTimer(this)),
    painter(this), game_state(GameState::START) {
    game = {};
    controller = {};
    connect(gameTimer, &QTimer::timeout, this, &DesktopView::updateGame);
    MemoryAllocation();
}

DesktopView::~DesktopView(){
    MemoryDeallocation();
}

void DesktopView::MemoryAllocation() {
    game.field = new int *[height]();
    for (int i = 0; i < height; ++i) {
        game.field[i] = new int[width]();
    }
    game.next = new int *[4]();
    for (int i = 0; i < 4; ++i) {
        game.next[i] = new int[4]();
    }
}

void DesktopView::MemoryDeallocation() {
    for (int i = 0; i < height; ++i) {
        delete[] game.field[i];
    }
    delete[] game.field;
    for (int i = 0; i < 4; ++i) {
        delete[] game.next[i];
    }
    delete[] game.next;
}

void DesktopView::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
}

void DesktopView::resizeGL(int w, int h) {
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(0, width, height, 0, -1, 1);
}

void DesktopView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setClearColor(Qt::black);

    painter.begin(this); // Начало рисования
    MenuProcessing();
    painter.end(); // Конец рисования

    // signal = Signals::NONE;
}

void DesktopView::keyPressEvent(QKeyEvent *event) {
    SignalProcessing(event);

    update(); // Перерисовываем виджет, чтобы отразить изменения
    QOpenGLWidget::keyPressEvent(event);
}

void DesktopView::setClearColor(const QColor &color) {
    glClearColor(
        color.redF(),   // Красный компонент (0.0 - 1.0)
        color.greenF(), // Зеленый компонент (0.0 - 1.0)
        color.blueF(),  // Синий компонент (0.0 - 1.0)
        color.alphaF()  // Альфа компонент (0.0 - 1.0)
        );
}

void DesktopView::MenuProcessing() {
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
        close(); // Закрываем окно при выборе "Exit"
        break;
    default:
        break;
    }
}

void DesktopView::InitMenu() {
    static int menu_option = 0;
    bool enter = false;
    std::vector<std::string> options = {"Tetris", "Snake", "Exit"};

    switch (signal) {
    case Signals::UP:
        if (menu_option > 0) menu_option--;
        break;
    case Signals::DOWN:
        if (menu_option < (int)options.size() - 1) menu_option++;
        break;
    case Signals::ENTER:
        enter = true;
        ApplyChoice(menu_option);
        break;
    default:
        break;
    }

    if (!enter) DrawMenu(options, menu_option);
}

void DesktopView::SignalProcessing(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        signal = Signals::UP;
        break;
    case Qt::Key_Down:
        signal = Signals::DOWN;
        break;
    case Qt::Key_Left:
        signal = Signals::LEFT;
        break;
    case Qt::Key_Right:
        signal = Signals::RIGHT;
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        signal = Signals::ENTER;
        break;
    case Qt::Key_P:
        signal = Signals::PAUSE;
        break;
    case Qt::Key_Escape:
        signal = Signals::ESC;
        break;
    default:
        signal = Signals::NONE;
        break;
    }
    std::cout << "Signal: " << static_cast<int>(signal) << std::endl;
}

void DesktopView::ApplyChoice(int &choice) {
    switch (choice) {
    case 0:
        state = MenuState::TETRIS_GAME;
        break;
    case 1:
        state = MenuState::SNAKE_GAME;
        controller.setGame(state);
        break;
    case 2:
        state = MenuState::EXIT_MENU;
        break;
    default:
        break;
    }

    signal = Signals::NONE;
    update();
}

void DesktopView::DrawMenu(const std::vector<std::string> &options, int &menu_option) {
    static auto font = QFont("Georgia", 35);
    static auto font_selected = QFont("Georgia", 45);
    font_selected.setBold(true);

    QFontMetrics fontMetrics(font);
    QFontMetrics fontMetricsSelected(font_selected);

    int dy = fontMetrics.height(); // Высота строки
    int end = options.size();

    // Вычисляем центральные координаты для первой строки
    int totalHeight = dy * end; // Общая высота всех строк
    int y = (height - totalHeight) / 2; // Вертикальное центрирование

    for (int i = 0; i < end; ++i) {
        QString text = QString::fromStdString(options[i]);
        int textWidth = (i == menu_option) ? fontMetricsSelected.horizontalAdvance(text) : fontMetrics.horizontalAdvance(text);
        int x = (width - textWidth) / 2; // Горизонтальное центрирование

        if (i == menu_option) {
            painter.setFont(font_selected);
            painter.setPen(Qt::red);
        } else {
            painter.setFont(font);
            painter.setPen(Qt::white);
        }

        painter.drawText(x, y + i * dy, text);
    }
}

void DesktopView::draw(const GameInfo_t &game) {
    game_state = controller.GetCurrentGameState();
    switch (game_state) {
    case GameState::START:
        drawStartScreen(game_state);
        break;
    case GameState::PLAYING:
        drawGame(game);
        break;
    case GameState::PAUSE:
        drawPauseScreen(game_state);
        break;
    case GameState::GAMEOVER:
        drawGameOver(game);
        break;
    default:
        break;
    }
}

void DesktopView::StartSnakeGame() {

    // game_state = GameState::START; // Устанавливаем состояние старта
    updateGame(); // Обновление игры
    gameTimer->start(10); // 100 миллисекунд между обновлениями
}

void DesktopView::updateGame() {
    game_state = controller.GetCurrentGameState();
    if (game_state != GameState::EXIT) {
        std::cout << "Current Game State in View: " << static_cast<int>(controller.GetCurrentGameState()) << std::endl;
        controller.GameProcessing(signal);
        controller.getData(game);
        draw(game); // Отрисовка игры
        update(); // Перерисовка виджета
        signal = Signals::NONE;
    } else {
        state = MenuState::MENU;
        gameTimer->stop();
        update();
        // Возможно, закрыть текущий виджет или перейти к меню
    }
}

void DesktopView::drawStartScreen(const GameState &state) {
    if (state == GameState::START) {
        painter.setPen(Qt::white);
        painter.drawRect(10, 10, width - 20, height - 20);
        painter.drawText(QRect(10, 10, width - 20, height - 20), Qt::AlignCenter, "Press Enter to start");
    }
}

void DesktopView::drawGame(const GameInfo_t &game) {
    painter.setPen(Qt::white);

    // Draw game field
    int blockSize = 20; // Размер блока
    painter.drawRect(10, 10, 20*10, 20*20);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (game.field[i][j] == 5) {
                painter.fillRect(10 + j * blockSize, 10 + i * blockSize, blockSize, blockSize, Qt::green);
            } else if (game.field[i][j] == 7) {
                 painter.fillRect(10 + j * blockSize, 10 + i * blockSize, blockSize, blockSize, Qt::red);
            } /*else painter.drawRect(j * blockSize, i * blockSize, blockSize, blockSize);*/
        }
    }

    // Draw sidebar
    painter.drawText(width - 150, 20, QString("Score: %1").arg(game.score));
    painter.drawText(width - 150, 40, QString("Level: %1").arg(game.level));
    painter.drawText(width - 150, 60, QString("Speed: %1").arg(game.speed));
}

void DesktopView::drawPauseScreen(const GameState &state) {
    if (state == GameState::PAUSE) {
        painter.setPen(Qt::white);
        painter.drawRect(10, 10, width - 20, height - 20);
        painter.drawText(QRect(10, 10, width - 20, height - 20), Qt::AlignCenter, "Game is paused\nPress P to resume");
    }
}

void DesktopView::drawGameOver(const GameInfo_t &game) {
    painter.setPen(Qt::white);
    painter.fillRect(0, 0, width, height, Qt::black);
    painter.drawRect(10, 10, width - 20, height - 20);
    painter.drawText(QRect(10, 10, width - 20, height - 20), Qt::AlignCenter, QString("Game Over!\nScore: %1  Level: %2\nStart over? (Press Enter)").arg(game.score).arg(game.level));
}

}
