#include "desktopview.h"

namespace s21 {

DesktopView::DesktopView()
    : screen_width_(450),
      screen_height_(580),
      block_size_(28),
      border_offset_(2),
      board_begin_(10),
      signal_(Signals::kNone),
      menu_state_(MenuState::kMenu),
      game_timer_(new QTimer(this)),
      painter_(this),
      game_state_(GameState::kStart),
      color_map_{} {
  current_game_ = {};
  controller_ = {};
  connect(game_timer_, &QTimer::timeout, this, &DesktopView::UpdateGame);
  MemoryAllocation();
  InitColors();
  SetWindowTitle("BrickGames");
}

DesktopView::~DesktopView() { MemoryDeallocation(); }

void DesktopView::InitColors() {
  // Создаем словарь для соответствия значений и цветов
  color_map_[1] = Qt::cyan;
  color_map_[2] = Qt::blue;
  color_map_[3] = Qt::white;
  color_map_[4] = Qt::yellow;
  color_map_[5] = Qt::green;
  color_map_[6] = Qt::magenta;
  color_map_[7] = Qt::red;
}

void DesktopView::MemoryAllocation() {
  current_game_.field = new int *[screen_height_]();
  for (int i = 0; i < screen_height_; ++i) {
    current_game_.field[i] = new int[screen_width_]();
  }
  current_game_.next = new int *[4]();
  for (int i = 0; i < 4; ++i) {
    current_game_.next[i] = new int[4]();
  }
}

void DesktopView::MemoryDeallocation() {
  for (int i = 0; i < screen_height_; ++i) {
    delete[] current_game_.field[i];
  }
  delete[] current_game_.field;
  for (int i = 0; i < 4; ++i) {
    delete[] current_game_.next[i];
  }
  delete[] current_game_.next;
}

void DesktopView::SetWindowTitle(const QString &title) {
  QOpenGLWidget::setWindowTitle(title);
}

void DesktopView::initializeGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, screen_width_, screen_height_, 0, -1, 1);
}

void DesktopView::resizeGL(int w, int h) {
  glLoadIdentity();
  glViewport(0, 0, w, h);
  glOrtho(0, screen_width_, screen_height_, 0, -1, 1);
}

void DesktopView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SetClearColor(Qt::black);

  if (!painter_.isActive()) painter_.begin(this);  // Начало рисования
  MenuProcessing();
  painter_.end();  // Конец рисования
}

void DesktopView::keyPressEvent(QKeyEvent *event) {
  SignalProcessing(event);
  update();  // Перерисовываем виджет, чтобы отразить изменения
  QOpenGLWidget::keyPressEvent(event);
}

void DesktopView::SetClearColor(const QColor &color) {
  glClearColor(color.redF(),    // Красный компонент (0.0 - 1.0)
               color.greenF(),  // Зеленый компонент (0.0 - 1.0)
               color.blueF(),   // Синий компонент (0.0 - 1.0)
               color.alphaF()   // Альфа компонент (0.0 - 1.0)
  );
}

void DesktopView::MenuProcessing() {
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
    case MenuState::kExit_MENU:
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

  switch (signal_) {
    case Signals::kUp:
      if (menu_option > 0) menu_option--;
      break;
    case Signals::kDown:
      if (menu_option < (int)options.size() - 1) menu_option++;
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

void DesktopView::SignalProcessing(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Up:
      signal_ = Signals::kUp;
      break;
    case Qt::Key_Down:
      signal_ = Signals::kDown;
      break;
    case Qt::Key_Left:
      signal_ = Signals::kLeft;
      break;
    case Qt::Key_Right:
      signal_ = Signals::kRight;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      signal_ = Signals::kEnter;
      break;
    case Qt::Key_P:
      signal_ = Signals::kPause;
      break;
    case Qt::Key_Escape:
      signal_ = Signals::kEsc;
      break;
    default:
      signal_ = Signals::kNone;
      break;
  }
}

void DesktopView::ApplyChoice(int &choice) {
  switch (choice) {
    case 0:
      menu_state_ = MenuState::kTetrisGame;
      controller_.SetGame(menu_state_);
      SetWindowTitle("Tetris");
      break;
    case 1:
      menu_state_ = MenuState::kSnakeGame;
      controller_.SetGame(menu_state_);
      SetWindowTitle("Snake");
      break;
    case 2:
      menu_state_ = MenuState::kExit_MENU;
      break;
    default:
      break;
  }

  signal_ = Signals::kNone;
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
  const int y = (screen_height_ - totalHeight) / 2;

  for (int i = 0; i < end; ++i) {
    const QString text = QString::fromStdString(options[i]);
    const int textWidth = (i == menuOption)
                              ? fontMetricsSelected.horizontalAdvance(text)
                              : fontMetrics.horizontalAdvance(text);
    const int x = (screen_width_ - textWidth) / 2;

    if (i == menuOption) {
      painter_.setFont(fontSelected);
      painter_.setPen(Qt::red);
    } else {
      painter_.setFont(font);
      painter_.setPen(Qt::white);
    }

    painter_.drawText(x, y + i * dy, text);
  }
}

void DesktopView::Draw(const GameInfo_t &game) {
  game_state_ = controller_.GetCurrentGameState();
  if (game_state_ == GameState::kPlaying)
    DrawGame(game);
  else
    DrawGameStateScreen(game_state_, game);
}

void DesktopView::DrawGameStateScreen(const GameState &state,
                                      const GameInfo_t &game) {
  if (!painter_.isActive()) painter_.begin(this);
  static const QFont font("Arial", 25);
  static const QRect textRect(board_begin_, board_begin, screen_width_ - 20,
                              screen_height_ - 20);
  static QTextOption textOption;
  static std::once_flag initFlag;
  std::call_once(initFlag, []() {
    textOption.setAlignment(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textOption.setTabStopDistance(40);  // Увеличиваем расстояние между строками
  });

  painter_.setPen(Qt::white);
  painter_.setFont(font);

  QString text;
  switch (state) {
    case GameState::kStart:
      text = "Press Enter to start";
      break;
    case GameState::kPause:
      text = "Game is paused\n\nPress P to resume";
      break;
    case GameState::kGameOver:
      text =
          QString("Game Over!\nScore: %1  Level: %2\n\nPress Enter to restart")
              .arg(game.score)
              .arg(game.level);
      break;
    default:
      return;
  }

  painter_.drawText(textRect, text, textOption);
}

void DesktopView::StartTheGame() {
  UpdateGame();  // Обновление игры
  game_timer_->start(10);  // 10 миллисекунд между обновлениями
}

void DesktopView::UpdateGame() {
  game_state_ = controller_.GetCurrentGameState();
  if (game_state_ != GameState::kExit) {
    controller_.GameProcessing(signal_);
    controller_.getData(current_game_);
    Draw(current_game_);  // Отрисовка игры
    update();             // Перерисовка виджета
    signal_ = Signals::kNone;
  } else {
    menu_state_ = MenuState::kMenu;
    SetWindowTitle("BrickGames");
    game_timer_->stop();
    update();
  }
}

void DesktopView::DrawGameField(const GameInfo_t &game) {
  QPen blackPen(Qt::black);
  blackPen.setWidth(4);
  painter_.setPen(blackPen);

  for (int i = 0, value = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      value = game.field[i][j];
      if (color_map_.contains(value)) {
        QColor color = colorMap[value];
        painter_.drawRect(board_begin_ + j * block_size_ + border_offset_,
                          board_begin_ + i * block_size_ + border_offset_,
                          block_size_ - 2 * border_offset_,
                          block_size_ - 2 * border_offset_);
        painter_.fillRect(board_begin_ + j * block_size_ + border_offset_,
                          board_begin_ + i * block_size_ + border_offset_,
                          block_size_ - 2 * border_offset_,
                          block_size_ - 2 * border_offset_, color);
      }
    }
  }
}

void DesktopView::DrawNextFigure(const GameInfo_t &game) {
  painter_.setPen(Qt::black);
  for (int i = 0, value = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      value = game.next[i][j];
      if (color_map_.contains(value)) {
        QColor color = colorMap[value];
        painter_.drawRect(320 + j * block_size_ + border_offset_,
                          220 + i * block_size_ + border_offset_,
                          block_size_ - 2 * border_offset_,
                          block_size_ - 2 * border_offset_);
        painter_.fillRect(320 + j * block_size_ + border_offset_,
                          220 + i * block_size_ + border_offset_,
                          block_size_ - 2 * border_offset_,
                          block_size_ - 2 * border_offset_, color);
      }
    }
  }
}

void DesktopView::DrawSidebar(const GameInfo_t &game) {
  QFont font("Arial", 20);
  painter_.setFont(font);
  painter_.setPen(Qt::white);
  painter_.drawText(310 - border_offset_, 60 - border_offset_,
                    QString("Score: %1").arg(game.score));
  painter_.drawText(310 - border_offset_, 100 - border_offset_,
                    QString("Level: %1").arg(game.level));
  painter_.drawText(310 - border_offset_, 140 - border_offset_,
                    QString("Record: %1").arg(game.high_score));
  if (menu_state_ == MenuState::kSnakeGame)
    painter_.drawText(310 - border_offset_, 180 - border_offset_,
                      QString("Speed: %1").arg(game.speed));

  if (menu_state_ == MenuState::kTetrisGame) {
    DrawNextFigure(game);
  }
}

void DesktopView::drawOuterFrame() {
  QPen whitePen(Qt::white);
  whitePen.setWidth(4);
  painter_.setPen(whitePen);

  painter_.drawRect(board_begin_ - border_offset_,
                    board_begin_ - border_offset_,
                    block_size_ * 10 + 2 * border_offset_,
                    block_size_ * 20 + 2 * border_offset_);
  painter_.drawRect(board_begin_ * 30 - border_offset_,
                    board_begin_ - border_offset_,
                    block_size_ * 5 + 2 * border_offset_,
                    block_size_ * 20 + 2 * border_offset_);
}

void DesktopView::DrawGame(const GameInfo_t &game) {
  if (!painter_.isActive()) painter_.begin(this);

  // Draw outer frame
  drawOuterFrame();

  // Draw game field
  DrawGameField(game);

  // Draw sidebar
  DrawSidebar(game);
}

}  // namespace s21
