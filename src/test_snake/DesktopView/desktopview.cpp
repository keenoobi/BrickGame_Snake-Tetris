#include "desktopview.h"

namespace s21 {

DesktopView::DesktopView()
    : screenWidth(450),
      screenHeight(580),
      blockSize(28),
      borderOffset(2),
      board_begin(10),
      signal(Signals::NONE),
      state(MenuState::MENU),
      gameTimer(new QTimer(this)),
      painter(this),
      game_state(GameState::START),
      colorMap{} {
  currentGame = {};
  controller = {};
  connect(gameTimer, &QTimer::timeout, this, &DesktopView::updateGame);
  MemoryAllocation();
  InitColors();
  setWindowTitle("BrickGames");
}

DesktopView::~DesktopView() { MemoryDeallocation(); }

void DesktopView::InitColors() {
  // Создаем словарь для соответствия значений и цветов
  colorMap[1] = Qt::cyan;
  colorMap[2] = Qt::blue;
  colorMap[3] = Qt::white;
  colorMap[4] = Qt::yellow;
  colorMap[5] = Qt::green;
  colorMap[6] = Qt::magenta;
  colorMap[7] = Qt::red;
}

void DesktopView::MemoryAllocation() {
  currentGame.field = new int *[screenHeight]();
  for (int i = 0; i < screenHeight; ++i) {
    currentGame.field[i] = new int[screenWidth]();
  }
  currentGame.next = new int *[4]();
  for (int i = 0; i < 4; ++i) {
    currentGame.next[i] = new int[4]();
  }
}

void DesktopView::MemoryDeallocation() {
  for (int i = 0; i < screenHeight; ++i) {
    delete[] currentGame.field[i];
  }
  delete[] currentGame.field;
  for (int i = 0; i < 4; ++i) {
    delete[] currentGame.next[i];
  }
  delete[] currentGame.next;
}

void DesktopView::setWindowTitle(const QString &title) {
  QOpenGLWidget::setWindowTitle(title);
}

void DesktopView::initializeGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
}

void DesktopView::resizeGL(int w, int h) {
  glLoadIdentity();
  glViewport(0, 0, w, h);
  glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
}

void DesktopView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setClearColor(Qt::black);

  if (!painter.isActive()) painter.begin(this);  // Начало рисования
  MenuProcessing();
  painter.end();  // Конец рисования
}

void DesktopView::keyPressEvent(QKeyEvent *event) {
  SignalProcessing(event);
  update();  // Перерисовываем виджет, чтобы отразить изменения
  QOpenGLWidget::keyPressEvent(event);
}

void DesktopView::setClearColor(const QColor &color) {
  glClearColor(color.redF(),    // Красный компонент (0.0 - 1.0)
               color.greenF(),  // Зеленый компонент (0.0 - 1.0)
               color.blueF(),   // Синий компонент (0.0 - 1.0)
               color.alphaF()   // Альфа компонент (0.0 - 1.0)
  );
}

void DesktopView::MenuProcessing() {
  switch (state) {
    case MenuState::MENU:
      InitMenu();
      break;
    case MenuState::SNAKE_GAME:
      StartTheGame();
      break;
    case MenuState::TETRIS_GAME:
      StartTheGame();
      break;
    case MenuState::EXIT_MENU:
      close();  // Закрываем окно при выборе "Exit"
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
}

void DesktopView::ApplyChoice(int &choice) {
  switch (choice) {
    case 0:
      state = MenuState::TETRIS_GAME;
      controller.setGame(state);
      setWindowTitle("Tetris");
      break;
    case 1:
      state = MenuState::SNAKE_GAME;
      controller.setGame(state);
      setWindowTitle("Snake");
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

void DesktopView::DrawMenu(const std::vector<std::string> &options,
                           int &menuOption) {
  static const QFont font("Georgia", 35);
  static const QFont fontSelected("Georgia", 45);
  static const QFontMetrics fontMetrics(font);
  static const QFontMetrics fontMetricsSelected(fontSelected);

  const int dy = fontMetricsSelected.height();
  const int end = options.size();
  const int totalHeight = dy * end;
  const int y = (screenHeight - totalHeight) / 2;

  for (int i = 0; i < end; ++i) {
    const QString text = QString::fromStdString(options[i]);
    const int textWidth = (i == menuOption)
                              ? fontMetricsSelected.horizontalAdvance(text)
                              : fontMetrics.horizontalAdvance(text);
    const int x = (screenWidth - textWidth) / 2;

    if (i == menuOption) {
      painter.setFont(fontSelected);
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
  if (game_state == GameState::PLAYING)
    drawGame(game);
  else
    drawGameStateScreen(game_state, game);
}

void DesktopView::drawGameStateScreen(const GameState &state,
                                      const GameInfo_t &game) {
  if (!painter.isActive()) painter.begin(this);
  static const QFont font("Arial", 25);
  static const QRect textRect(board_begin, board_begin, screenWidth - 20,
                              screenHeight - 20);
  static QTextOption textOption;
  static std::once_flag initFlag;
  std::call_once(initFlag, []() {
    textOption.setAlignment(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textOption.setTabStopDistance(40);  // Увеличиваем расстояние между строками
  });

  painter.setPen(Qt::white);
  painter.setFont(font);

  QString text;
  switch (state) {
    case GameState::START:
      text = "Press Enter to start";
      break;
    case GameState::PAUSE:
      text = "Game is paused\n\nPress P to resume";
      break;
    case GameState::GAMEOVER:
      text =
          QString("Game Over!\nScore: %1  Level: %2\n\nPress Enter to restart")
              .arg(game.score)
              .arg(game.level);
      break;
    default:
      return;
  }

  painter.drawText(textRect, text, textOption);
}

void DesktopView::StartTheGame() {
  updateGame();  // Обновление игры
  gameTimer->start(10);  // 10 миллисекунд между обновлениями
}

void DesktopView::updateGame() {
  game_state = controller.GetCurrentGameState();
  if (game_state != GameState::EXIT) {
    controller.GameProcessing(signal);
    controller.getData(currentGame);
    draw(currentGame);  // Отрисовка игры
    update();           // Перерисовка виджета
    signal = Signals::NONE;
  } else {
    state = MenuState::MENU;
    setWindowTitle("BrickGames");
    gameTimer->stop();
    update();
  }
}

void DesktopView::drawGameField(const GameInfo_t &game) {
  QPen blackPen(Qt::black);
  blackPen.setWidth(4);
  painter.setPen(blackPen);

  for (int i = 0, value = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      value = game.field[i][j];
      if (colorMap.contains(value)) {
        QColor color = colorMap[value];
        painter.drawRect(board_begin + j * blockSize + borderOffset,
                         board_begin + i * blockSize + borderOffset,
                         blockSize - 2 * borderOffset,
                         blockSize - 2 * borderOffset);
        painter.fillRect(board_begin + j * blockSize + borderOffset,
                         board_begin + i * blockSize + borderOffset,
                         blockSize - 2 * borderOffset,
                         blockSize - 2 * borderOffset, color);
      }
    }
  }
}

void DesktopView::drawNextFigure(const GameInfo_t &game) {
  painter.setPen(Qt::black);
  for (int i = 0, value = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      value = game.next[i][j];
      if (colorMap.contains(value)) {
        QColor color = colorMap[value];
        painter.drawRect(320 + j * blockSize + borderOffset,
                         180 + i * blockSize + borderOffset,
                         blockSize - 2 * borderOffset,
                         blockSize - 2 * borderOffset);
        painter.fillRect(320 + j * blockSize + borderOffset,
                         180 + i * blockSize + borderOffset,
                         blockSize - 2 * borderOffset,
                         blockSize - 2 * borderOffset, color);
      }
    }
  }
}

void DesktopView::drawSidebar(const GameInfo_t &game) {
  QFont font("Arial", 20);
  painter.setFont(font);
  painter.setPen(Qt::white);
  painter.drawText(310 - borderOffset, 60 - borderOffset,
                   QString("Score: %1").arg(game.score));
  painter.drawText(310 - borderOffset, 100 - borderOffset,
                   QString("Level: %1").arg(game.level));
  if (state == MenuState::SNAKE_GAME)
    painter.drawText(310 - borderOffset, 140 - borderOffset,
                     QString("Speed: %1").arg(game.speed));

  if (state == MenuState::TETRIS_GAME) {
    drawNextFigure(game);
  }
}

void DesktopView::drawOuterFrame() {
  QPen whitePen(Qt::white);
  whitePen.setWidth(4);
  painter.setPen(whitePen);

  painter.drawRect(board_begin - borderOffset, board_begin - borderOffset,
                   blockSize * 10 + 2 * borderOffset,
                   blockSize * 20 + 2 * borderOffset);
  painter.drawRect(board_begin * 30 - borderOffset, board_begin - borderOffset,
                   blockSize * 5 + 2 * borderOffset,
                   blockSize * 20 + 2 * borderOffset);
}

void DesktopView::drawGame(const GameInfo_t &game) {
  if (!painter.isActive()) painter.begin(this);

  // Draw outer frame
  drawOuterFrame();

  // Draw game field
  drawGameField(game);

  // Draw sidebar
  drawSidebar(game);
}

}  // namespace s21
