#include "../include/Model.hpp"

namespace s21 {
constexpr int kBaseSnakeSize = 4;

SnakeModel::SnakeModel(int width, int height)
    : width_(width),
      height_(height),
      score_(0),
      level_(1),
      speed_(300),
      snake_moved(false),
      accelerationSpeed(100),
      acceleration(false),
      game_board_(height, std::vector<int>(width, 0)),
      currentState(GameState::kStart) {
  last_update_time_ = GetCurrentTimeInMilliseconds();
}

void SnakeModel::ResetGame() {
  snake.clear();
  int startX = width_ / 2 - 1;
  int startY = height_ / 2;
  for (int i = 0; i < kBaseSnakeSize; ++i) {
    snake.push_back(Point(startX + i, startY));
  }
  GenerateFood();
  score_ = 0;
  level_ = 1;
  speed_ = 300;
  game_board_ =
      std::vector<std::vector<int>>(height_, std::vector<int>(width_, 0));
  current_direction_ = Direction::kLeft;
  currentState = GameState::kStart;
  UpdateGameBoard();
}

void SnakeModel::GetData(GameInfo_t& game) const {
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      game.field[i][j] = game_board_[i][j];
    }
  }
  game.score = score_;
  game.level = level_;
  game.speed = speed_;
}

void SnakeModel::HandleEvent(Signals signal) {
  action act =
      fsm_table_[static_cast<int>(currentState)][static_cast<int>(signal)];

  if (act) (this->*act)();
}

void SnakeModel::GenerateFood() {
  do {
    food_.x = rand() % width_;
    food_.y = rand() % height_;
  } while (CollisionWithBody(food_));
}

void SnakeModel::MoveSnake() {
  Point newHead = CalculateNewHead(snake.front());

  if (OutOfBounds(newHead) || CollisionWithBody(newHead)) {
    setCurrentState(GameState::kGameOver);
    return;
  }

  bool foodEaten = (newHead == food_);

  // Удаляем хвост, если змейка не съела еду
  if (!foodEaten) {
    game_board_[snake.back().y][snake.back().x] = 0;
    snake.pop_back();
  } else if (foodEaten) {
    HandleFoodConsumption();
  }

  // Добавляем новую голову
  snake.insert(snake.begin(), newHead);

  UpdateGameBoard();
}

Point SnakeModel::CalculateNewHead(const Point& head) {
  Point newHead(head.x, head.y);

  switch (current_direction_) {
    case Direction::kUp:
      newHead.y--;
      break;
    case Direction::kDown:
      newHead.y++;
      break;
    case Direction::kLeft:
      newHead.x--;
      break;
    case Direction::kRight:
      newHead.x++;
      break;
  }

  return newHead;
}

bool SnakeModel::OutOfBounds(const Point& newHead) {
  return newHead.x < 0 || newHead.x >= width_ || newHead.y < 0 ||
         newHead.y >= height_;
}

bool SnakeModel::CollisionWithBody(const Point& newHead) {
  for (const auto& part : snake) {
    if (part == newHead) {
      return true;
    }
  }
  return false;
}

void SnakeModel::HandleFoodConsumption() {
  score_++;
  if (score_ % 5 == 0 && level_ < 10 && speed_) {
    level_++;
    speed_ -= 40;  // Увеличиваем скорость с каждым уровнем
  }
  GenerateFood();
}

void SnakeModel::UpdateGameBoard() {
  for (const auto& part : snake) {
    game_board_[part.y][part.x] = 5;  // Змейка будет представлена 5
  }
  game_board_[snake.front().y][snake.front().x] = 4;  // Голова змейки 4
  game_board_[food_.y][food_.x] = 7;  // Еда будет представлена 7
}

long long SnakeModel::GetCurrentTimeInMilliseconds() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch());
  return duration.count();
}

const std::vector<Point>& SnakeModel::getSnake() const { return snake; }
const Point& SnakeModel::GetFood() const { return food_; }
Direction SnakeModel::GetCurrentDirection() const { return current_direction_; }
void SnakeModel::SetCurrentDirection(Direction dir) {
  current_direction_ = dir;
}
GameState SnakeModel::GetCurrentState() const { return currentState; }
void SnakeModel::setCurrentState(GameState state) { currentState = state; }

inline bool SnakeModel::CurrentDirectionIs(Direction dir) const {
  return current_direction_ != dir;
}

void SnakeModel::MoveForward() {
  long long currentTime = GetCurrentTimeInMilliseconds();
  if (currentTime - last_update_time_ >= speed_) {
    MoveSnake();
    last_update_time_ = currentTime;
    snake_moved = true;
  }
}

void SnakeModel::moveUp() {
  if (snake_moved && CurrentDirectionIs(Direction::kDown)) {
    if (CurrentDirectionIs(Direction::kUp)) {
      SetCurrentDirection(Direction::kUp);
      snake_moved = false;
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveDown() {
  if (snake_moved && CurrentDirectionIs(Direction::kUp)) {
    if (CurrentDirectionIs(Direction::kDown)) {
      SetCurrentDirection(Direction::kDown);
      snake_moved = false;
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveRight() {
  if (snake_moved && CurrentDirectionIs(Direction::kLeft)) {
    if (CurrentDirectionIs(Direction::kRight)) {
      SetCurrentDirection(Direction::kRight);
      snake_moved = false;
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveLeft() {
  if (snake_moved && CurrentDirectionIs(Direction::kRight)) {
    if (CurrentDirectionIs(Direction::kLeft)) {
      SetCurrentDirection(Direction::kLeft);
      snake_moved = false;
    } else
      MoveSnake();
  }
}

void SnakeModel::Start() {
  ResetGame();
  setCurrentState(GameState::kPlaying);
}

void SnakeModel::Exit() { setCurrentState(GameState::kExit); }

void SnakeModel::Pause() {
  if (currentState != GameState::kPause) {
    setCurrentState(GameState::kPause);
  } else {
    setCurrentState(GameState::kPlaying);
  }
}

void SnakeModel::GameOver() { setCurrentState(GameState::kGameOver); }

}  // namespace s21
