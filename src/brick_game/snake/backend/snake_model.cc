#include "snake_model.h"

namespace s21 {

SnakeModel::SnakeModel()
    : width_(10),
      height_(20),
      score_(0),
      level_(1),
      speed_(300),
      record_(0),
      snake_moved_(false),
      food_eaten_(false),
      current_direction_(Direction::kLeft),
      game_board_(height_, std::vector<int>(width_, 0)),
      currentState(GameState::kStart) {
  last_update_time_ = GetCurrentTimeInMilliseconds();
  LoadSnakeRecord();
}

void SnakeModel::ResetGame() {
  snake_.clear();
  int startX = width_ / 2 - 1;
  int startY = height_ / 2;
  for (int i = 0; i < kBaseSnakeSize; ++i) {
    snake_.push_back(Point(startX + i, startY));
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
  game.high_score = record_;
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
  } while (CollisionWithBody(food_) || game_board_[food_.y][food_.x] != 0);
}

void SnakeModel::MoveSnake() {
  Point newHead = CalculateNewHead(snake_.front());

  if (OutOfBounds(newHead) || CollisionWithBody(newHead)) {
    setCurrentState(GameState::kGameOver);
    return;
  }

  SetFoodEaten(newHead == food_);

  // Удаляем хвост, если змейка не съела еду
  if (!food_eaten_) {
    game_board_[snake_.back().y][snake_.back().x] = 0;
    snake_.pop_back();
  } else {
    HandleFoodConsumption();
  }
  // Добавляем новую голову
  snake_.push_front(newHead);

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
  for (const auto& part : snake_) {
    if (part == newHead) {
      return true;
    }
  }
  return false;
}

void SnakeModel::HandleFoodConsumption() {
  score_++;
  if (score_ == 200) {
    setCurrentState(GameState::kGameOver);
    return;
  } else if (score_ % kPointsPerLevel == 0 && level_ < kMaxLevel && speed_) {
    level_++;
    speed_ -= 21;  // Увеличиваем скорость с каждым уровнем
  }

  if (score_ > record_) {
    record_ = score_;
    WriteSnakeRecord();
  }
  GenerateFood();
}

void SnakeModel::UpdateGameBoard() {
  // Очищаем игровую доску
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      game_board_[i][j] = 0;
    }
  }

  for (const auto& part : snake_) {
    game_board_[part.y][part.x] = 5;  // Змейка будет представлена 5
  }
  game_board_[snake_.front().y][snake_.front().x] = 4;  // Голова змейки 4
  game_board_[food_.y][food_.x] = 7;  // Еда будет представлена 7
}

long long SnakeModel::GetCurrentTimeInMilliseconds() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch());
  return duration.count();
}

const std::deque<Point>& SnakeModel::GetSnake() const { return snake_; }
const Point& SnakeModel::GetFood() const { return food_; }
Direction SnakeModel::GetCurrentDirection() const { return current_direction_; }
void SnakeModel::SetCurrentDirection(Direction dir) {
  current_direction_ = dir;
}
GameState SnakeModel::GetCurrentState() const { return currentState; }
void SnakeModel::setCurrentState(GameState state) { currentState = state; }

inline bool SnakeModel::CurrentDirectionIs(Direction dir) const {
  return current_direction_ == dir;
}
inline void SnakeModel::setSnakeMoved(bool status) { snake_moved_ = status; }
inline void SnakeModel::SetFoodEaten(bool status) { food_eaten_ = status; }
void SnakeModel::setFood(int x, int y) {
  food_.x = x;
  food_.y = y;
}

void SnakeModel::MoveForward() {
  long long currentTime = GetCurrentTimeInMilliseconds();
  if (currentTime - last_update_time_ >= speed_) {
    MoveSnake();
    last_update_time_ = currentTime;
    setSnakeMoved(true);
  }
}

void SnakeModel::MoveUp() {
  if (snake_moved_ && !CurrentDirectionIs(Direction::kDown)) {
    if (!CurrentDirectionIs(Direction::kUp)) {
      SetCurrentDirection(Direction::kUp);
      setSnakeMoved(false);
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveDown() {
  if (snake_moved_ && !CurrentDirectionIs(Direction::kUp)) {
    if (!CurrentDirectionIs(Direction::kDown)) {
      SetCurrentDirection(Direction::kDown);
      setSnakeMoved(false);
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveRight() {
  if (snake_moved_ && !CurrentDirectionIs(Direction::kLeft)) {
    if (!CurrentDirectionIs(Direction::kRight)) {
      SetCurrentDirection(Direction::kRight);
      setSnakeMoved(false);
    } else
      MoveSnake();
  }
}

void SnakeModel::MoveLeft() {
  if (snake_moved_ && !CurrentDirectionIs(Direction::kRight)) {
    if (!CurrentDirectionIs(Direction::kLeft)) {
      SetCurrentDirection(Direction::kLeft);
      setSnakeMoved(false);
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

void SnakeModel::LoadSnakeRecord() {
  std::ifstream file(kFileName, std::ios::binary);
  if (file.is_open()) {
    file.read(reinterpret_cast<char*>(&record_), sizeof(record_));
    file.close();
  }
}

void SnakeModel::WriteSnakeRecord() {
  std::ofstream file(kFileName, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(&record_), sizeof(record_));
    file.close();
  }
}

}  // namespace s21
